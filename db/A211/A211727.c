/*
 * A211727  Number of -3..3 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having one,
 *          two, three, four or five distinct values for every i,j,k<=n.
 * https://oeis.org/A211727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 22, u64 n <= 47, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A211727: Number of -3..3 arrays satisfying the 5‑value condition.
   Empirical recurrence of order 19
   a(n) = 8*a(n‑1) –13*a(n‑2) –54*a(n‑3) +187*a(n‑4) +44*a(n‑5)
           –781*a(n‑6) +486*a(n‑7) +1514*a(n‑8) –1663*a(n‑9) –1455*a(n‑10)
           +2317*a(n‑11) +629*a(n‑12) –1634*a(n‑13) –68*a(n‑14) +606*a(n‑15)
           –22*a(n‑16) –112*a(n‑17) +4*a(n‑18) +8*a(n‑19)
   Initial values for 1 ≤ n ≤ 19 are
   49, 131, 311, 719, 1627, 3679, 8283, 18713, 42325, 96169,
   219263, 502397, 1156113, 2673435, 6209225, 14485465,
   33927541, 79766671, 188169795. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: ensure the buffer has at least 'needed' elements.  */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap < needed) {
        size_t new_cap = needed > (*cap << 1) ? needed : (*cap << 1);
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
}

/* Empirical recurrence coefficients for order 19   */
static const int coeff[19] = {
    8,   -13, -54, 187, 44, -781, 486, 1514,
   -1663, -1455, 2317, 629, -1634, -68, 606,
   -22, -112, 4, 8
};

/* Initial values (19 terms) stored as uint64_t, cast to seq_t when used. */
static const uint64_t init64[19] = {
    49ULL, 131ULL, 311ULL, 719ULL, 1627ULL, 3679ULL,
    8283ULL, 18713ULL, 42325ULL, 96169ULL,
    219263ULL, 502397ULL, 1156113ULL, 2673435ULL,
    6209225ULL, 14485465ULL, 33927541ULL,
    79766671ULL, 188169795ULL
};

static seq_t *cache = NULL;
static size_t cache_len = 0;
static idx_t cache_max = 0;

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Ensure buffer holds indices 0..n */
    size_t needed = (size_t)n + 1;
    ensure_capacity(needed, &cache_len, (void **)&cache, sizeof(seq_t));

    /* Compute missing values up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 19) {
            /* cast initial value; if it overflows seq_t the behaviour is defined by truncation */
            cache[i] = (seq_t)init64[i - 1];
        } else {
            /* Use 128-bit intermediate to avoid overflow when seq_t is small.
               The final result is casted to seq_t. */
            __int128 sum = 0;
            for (int k = 0; k < 19; ++k) {
                int c = coeff[k];
                __int128 term = (__int128)cache[i - 1 - k] * c;
                sum += term;
            }
            cache[i] = (seq_t)sum;
        }
    }
    cache_max = n;
    return cache[n];
}
