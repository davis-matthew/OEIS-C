/*
 * A170092  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for 1 ≤ n ≤ 36: a(n) = 33 * Σ_{i=0}^{n-1} a(i) + 2;
 *            a(37) = 33 * Σ_{i=1}^{36} a(i) – 561 * a(0) + 1; for n ≥ 38:
 *            a(n) = 33 * Σ_{i=n-36}^{n-1} a(i) – 561 * a(n-37). (All sums are
 *            over previous terms; for n < 36 the sum runs to the available
 *            terms only.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* sequence values 0 .. last */
    static idx_t  len   = 0;         /* allocated length of cache */
    static idx_t  last  = 0;         /* highest index computed so far */
    static seq_t  win32 = 0;         /* window sum of the last up to 36 terms */

    /* Ensure storage for at least (n+1) terms */
    if (len <= n) {
        idx_t new_len = (len ? len*2 : n+1);
        while (new_len <= n) new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(1);                 /* allocation failed */
        for (idx_t i = len; i < new_len; ++i) tmp[i] = 0;
        cache = tmp;
        len   = new_len;
    }

    /* Initialise the first value if not yet done */
    if (cache[0] == 0) {
        cache[0] = 1;
        win32 = cache[0];
        last = 0;
    }

    /* If we already have a[n], just return it */
    if (n <= last) return cache[n];

    /* Compute successive terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term;
        if (i <= 36) {
            /* a(i) = 33 * sum_{k=0}^{i-1} a(k) + 2 */
            term = mul_small(win32, 33);
            term += (seq_t)2;
        } else if (i == 37) {
            /* a(37) = 33 * sum_{k=1}^{36} a(k) - 561 * a(0) + 1 */
            term = mul_small(win32, 33);
            term -= mul_small(cache[0], 561);
            term += (seq_t)1;
        } else { /* i >= 38 */
            /* a(i) = 33 * sum_{k=i-36}^{i-1} a(k) - 561 * a(i-37) */
            term = mul_small(win32, 33);
            term -= mul_small(cache[i-37], 561);
        }

        cache[i] = term;

        /* Update the sliding window of the last 36 terms */
        win32 += cache[i];
        if (i >= 36) win32 -= cache[i-36];
    }

    last = n;
    return cache[n];
}
