/*
 * A077122  Let M_n be the n X n matrix M_(i,j) = 2^i-2^j then the
 *          characteristic polynomial of M_n = x^n-a(n)*x^(n-2).
 * https://oeis.org/A077122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 53*a(n-2) + 106*a(n-3) - 96*a(n-4) +
 *            32*a(n-5) for n>4 Initial values: a(0)=0, a(1)=4, a(2)=56,
 *            a(3)=460, a(4)=2976
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamically cached sequence values   */
static idx_t  cache_len = 0;         /* number of pre‑computed entries       */

seq_t a(idx_t n)
{
    /* Initialise base values once */
    if (cache_len == 0) {
        cache_len = 5;                      /* we need indices 0..4          */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failed                */
        cache[0] = 0;
        cache[1] = 4;
        cache[2] = 56;
        cache[3] = 460;
        cache[4] = 2976;
    }

    /* Return if already computed */
    if (n < cache_len)
        return cache[n];

    /* Resize cache to accommodate up to n */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;                    /* reallocation failed                */
    cache = tmp;

    /* Compute missing values using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term = 12 * cache[i-1];
        term -= 53 * cache[i-2];
        term += 106 * cache[i-3];
        term -= 96 * cache[i-4];
        term += 32 * cache[i-5];
        cache[i] = term;
    }

    cache_len = new_len;
    return cache[n];
}
