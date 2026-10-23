/*
 * A037359  Numbers whose base-5 and base-7 expansions have no digits in
 *          common.
 * https://oeis.org/A037359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer whose base‑5 and base‑7
 *            representations have no digit in common. Implementation:
 *            brute‑force enumeration with memoization
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static memoization state */
    static seq_t *cache = NULL;   /* array of accepted terms */
    static idx_t cache_cnt = 0;   /* number of terms stored */
    static idx_t cache_cap = 0;   /* allocated capacity of cache */
    static seq_t cur = 1;         /* next integer to test */

    /* initialize cache if necessary */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* generate terms until we have at least n terms */
    while (cache_cnt < n) {
        seq_t x = cur;

        /* compute digit masks for base-5 and base-7 representations */
        uint8_t m5 = 0, m7 = 0;
        seq_t t = x;
        do {
            uint8_t d = (uint8_t)(t % 5);
            m5 |= 1u << d;
            t /= 5;
        } while (t);
        t = x;
        do {
            uint8_t d = (uint8_t)(t % 7);
            m7 |= 1u << d;
            t /= 7;
        } while (t);

        /* accept if digit sets are disjoint */
        if ((m5 & m7) == 0) {
            if (cache_cnt == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_cnt++] = x;
        }
        ++cur;
    }

    return cache[n - 1];
}
