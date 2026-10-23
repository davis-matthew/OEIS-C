/*
 * A141544  Odd numbers k such that 2k+5 is a prime.
 * https://oeis.org/A141544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Defines a(n) = the nth odd integer k such that 2k+5 is prime. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static idx_t next_k = 1;          /* next odd candidate to test */

    if (n == 0)          /* a(0) is undefined – return 0 as a safe value */
        return 0;

    while (cache_len < n) {
        /* search forwards for the next odd k with 2k+5 prime */
        while (1) {
            seq_t p = (seq_t)next_k * 2 + 5;   /* 2k+5 */

            if (is_prime_big(p)) {
                /* ensure the cache has space */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();          /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next_k;   /* store the odd k */
                next_k += 2;                         /* advance to next odd */
                break;
            }
            next_k += 2; /* skip the even numbers */
        }
    }

    return cache[n - 1];   /* 0‑based internal indexing */
}
