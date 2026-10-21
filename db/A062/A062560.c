/*
 * A062560  The first few denominators in the odd-greedy expansion of 5/139.
 * https://oeis.org/A062560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn
 * Formula:   the odd‑greedy Egyptian fraction decomposition of 5/139. Each
 *            term a(n) is the smallest odd integer greater than or equal to
 *            ceil( cur_den / cur_num ) at the (n+1)‑st step, with the
 *            remainder reduced by GCD each time. The sequence terminates
 *            because 5/139 is a rational number
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Static cache of already computed denominators. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* The current remainder starts as the fraction 5/139. */
    static seq_t cur_num = (seq_t)5;
    static seq_t cur_den = (seq_t)139;

    /* Allocate cache on first call. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n+1 of them. */
    while (cache_len <= n) {
        /* Compute the next odd denominator d. */
        seq_t d = cur_den / cur_num;
        if (cur_den % cur_num != (seq_t)0)
            d += (seq_t)1;                     /* ceil */
        if ((d & (seq_t)1) == (seq_t)0)       /* make odd */
            d += (seq_t)1;

        /* Grow cache if needed. */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* Store the denominator. */
        cache[cache_len++] = d;

        /* Update the remainder: cur_num/cur_den -= 1/d. */
        seq_t new_num = cur_num * d - cur_den;
        seq_t new_den = cur_den * d;
        seq_t g = gcd(new_num, new_den);      /* use provided helper */
        cur_num = new_num / g;
        cur_den = new_den / g;

        /* Terminate if the fraction is exactly zero. */
        if (cur_num == (seq_t)0)
            break;
    }

    return (n < cache_len) ? cache[n] : (seq_t)0;
}
