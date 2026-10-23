/*
 * A024909  Numbers k such that 9*k - 4 is prime.
 * https://oeis.org/A024909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer k such that 9*k - 4 is prime. (a(1)=1,
 *            a(2)=3, …)
 * Range:     u8 n <= 9, u16 n <= 812, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cap  = 0;   /* allocated elements */
    static size_t sz   = 0;   /* number of terms stored */
    static idx_t next_k = 1;  /* next candidate k to test */

    if (n == 0) return 0;  /* offset does not matter for n < 1 */

    /* extend cache until we have at least n terms */
    while (sz < n) {
        seq_t k = (seq_t)next_k;
        seq_t p = k * 9 - 4;                /* 9*k - 4 */
        if (is_prime_big(p)) {
            /* ensure capacity for another term */
            if (sz + 1 > cap) {
                size_t new_cap = cap ? cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;                 /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[sz++] = k;    /* store the valid k */
        }
        ++next_k;
    }

    return cache[n - 1];
}
