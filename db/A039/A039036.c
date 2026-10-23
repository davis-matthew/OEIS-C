/*
 * A039036  Numbers whose base-7 representation has the same number of 1's and
 *          3's.
 * https://oeis.org/A039036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th nonnegative integer such that the number of 1’s
 *            equals the number of 3’s in its base‑7 representation
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) {                 /* offset starts at 1, just in case */
        return 0;
    }

    /* static cache to store already found terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;
    static seq_t next_to_test = 0;   /* next nonnegative integer to examine */

    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* initial allocation if not yet done */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) {
            return 0; /* allocation failed – never happens in normal use */
        }
    }

    /* extend cache until it contains the n-th term */
    while (cache_size < n) {
        seq_t x = next_to_test;
        /* count 1's and 3's in base 7 */
        int c1 = 0, c3 = 0;
        seq_t temp = x;
        while (temp > 0) {
            int d = (int)(temp % 7);
            if (d == 1) {
                c1++;
            } else if (d == 3) {
                c3++;
            }
            temp /= 7;
        }
        /* 0 qualifies because the loop is skipped and c1 == c3 == 0 */
        if (c1 == c3) {
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
                if (tmp == NULL) {
                    return 0; /* failed reallocation – unlikely */
                }
                cache = tmp;
            }
            cache[cache_size++] = x;
        }
        next_to_test++;
    }

    return cache[n - 1];
}
