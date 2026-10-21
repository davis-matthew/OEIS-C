/*
 * A230615  Number of 3Xn 0..2 arrays x(i,j) with each element horizontally or
 *          antidiagonally next to at least one element with value (x(i,j)+1)
 *          mod 3, and upper left element zero.
 * https://oeis.org/A230615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical linear recurrence of order 14 a(n) = 15*a(n-1)
 *            -64*a(n-2) +61*a(n-3) +181*a(n-4) -210*a(n-5) -679*a(n-6)
 *            +577*a(n-7) +2166*a(n-8) -2571*a(n-9) -789*a(n-10) +760*a(n-11)
 *            +618*a(n-12) +264*a(n-13) +56*a(n-14) for n > 17 Initial terms
 *            (n = 1..17): 0, 0, 0, 30, 260, 2358, 20430, 176774, 152...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_vals[17] = {
    0ULL, 0ULL, 0ULL, 30ULL, 260ULL, 2358ULL, 20430ULL,
    176774ULL, 1527918ULL, 13204026ULL, 114099558ULL,
    985963206ULL, 8519922122ULL, 73622499446ULL,
    636187953758ULL, 5497437822074ULL, 47504550336526ULL
};

static const int coeffs[14] = {15, -64, 61, 181, -210, -679,
                               577, 2166, -2571, -789, 760, 618,
                               264, 56};

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure values up to index n are computed in cache */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;

    seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* Fill initial terms if needed */
    if (cache_len < 17) {
        idx_t start = cache_len;
        for (idx_t i = start; i < 17 && i < n; ++i)
            cache[i] = init_vals[i];
        cache_len = 17;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t val = 0;
        for (int k = 1; k <= 14; ++k) {
            seq_t term = cache[i - k];
            int c = coeffs[k - 1];
            seq_t prod;
            if (c >= 0) {
                prod = term * (seq_t)c;
                val += prod;
            } else {
                prod = term * (seq_t)(-c);
                val -= prod;
            }
        }
        cache[i] = val;
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, return 0 for safety */
    ensure_cache(n);
    return cache[n - 1];
}
