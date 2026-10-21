/*
 * A052559  Expansion of e.g.f. (1-x)/(1 - 2*x - x^2 + x^3).
 * https://oeis.org/A052559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*a(n‑1) + n*(n‑1)*a(n‑2) – n*(n‑1)*(n‑2)*a(n‑3)
 *            Initial values: a(0) = 1, a(1) = 1, a(2) = 6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* memoised values */
static idx_t  a_cap   = 0;      /* allocated number of slots in a_cache */
static idx_t  a_len   = 0;      /* number of values already computed (next unused index) */

/* Ensure a_cache can hold indices up to n inclusive. */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= a_cap)
        return;

    idx_t new_cap = a_cap ? a_cap * 2 : 3;
    while (new_cap <= n)
        new_cap *= 2;

    /* realloc may return NULL if the allocation fails; in that case
       the program will abort on the next write, which is acceptable for
       the contest environment.  Cast to seq_t* for C++ compatibility. */
    a_cache = (seq_t *)realloc(a_cache, sizeof(seq_t) * new_cap);
    a_cap   = new_cap;
}

/* Compute all values up to index n using the recurrence. */
static void compute_up_to(idx_t n)
{
    for (idx_t i = a_len; i <= n; ++i) {
        if (i == 0) {
            a_cache[0] = (seq_t)1;
        } else if (i == 1) {
            a_cache[1] = (seq_t)1;
        } else if (i == 2) {
            a_cache[2] = (seq_t)6;
        } else {
            seq_t nn   = (seq_t)i;
            seq_t t1   = 2 * nn * a_cache[i - 1];
            seq_t t2   = nn * (nn - 1) * a_cache[i - 2];
            seq_t t3   = nn * (nn - 1) * (nn - 2) * a_cache[i - 3];
            a_cache[i] = t1 + t2 - t3;
        }
    }
    a_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    if (n >= a_len) {
        compute_up_to(n);
    }
    return a_cache[n];
}
