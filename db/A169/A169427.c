/*
 * A169427  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * G(x) = (1 + 2*∑_{k=1}^{31} x^k + x^32) / * (1 -
 *            28*∑_{k=1}^{31} x^k + 406*x^32) * * Recurrence (valid for all n
 *            ≥ 0): * a(0) = 1 * a(n) = 28*∑_{k=1}^{31} a(n-k) - 406*a(n-32) +
 *            rhs(n) * * where rhs(n) = { 1 if n==0, * 2 if 1 ≤ n ≤ 31, * 1 if
 *            n==32, * 0 otherwise }. * When n-k < 0, the term a(n-...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t  cap   = 0;     /* allocated capacity */
    static size_t  len   = 0;     /* number of terms already in cache */

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute terms up to n (inclusive) */
    for (idx_t i = len; i <= n; ++i) {
        /* Sum of the previous up to 31 terms */
        seq_t s = (seq_t)0;
        for (idx_t k = 1; k <= 31; ++k) {
            if (i >= k) s += cache[i - k];
        }

        seq_t val = (seq_t)28 * s;

        /* RHS contribution */
        seq_t rhs = (seq_t)0;
        if (i == 0)          rhs = (seq_t)1;
        else if (i <= 31)    rhs = (seq_t)2;
        else if (i == 32)    rhs = (seq_t)1;
        val += rhs;

        /* Subtract 406 * a(i-32) if i >= 32 */
        if (i >= 32) {
            val -= (seq_t)406 * cache[i - 32];
        }

        cache[i] = val;
    }

    len = (size_t)n + 1;
    return cache[n];
}
