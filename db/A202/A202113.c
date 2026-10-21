/*
 * A202113  Numbers k such that 90*k + 61 is prime.
 * https://oeis.org/A202113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer k (with a(1)=0) for which 90*k + 61 is
 *            prime
 * Range:     u8 n <= 3, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *cache   = NULL;   /* array of found k’s */
    static size_t cache_len = 0;    /* number of terms stored */
    static size_t cache_cap = 0;    /* current capacity of the array */
    static idx_t  next_k   = 0;     /* next candidate k to test */

    if (n == 0) return 0;   /* offset 1 – a(1) is the first term */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* Form 90*k + 61 */
        seq_t val = (seq_t)90 * next_k + 61;

        /* Test primality of the candidate */
        if (is_prime_big(val)) {
            /* Grow cache if needed */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* unrecoverable allocation failure */
                    abort();
                }
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len] = (seq_t)next_k;  /* store the k itself */
            cache_len++;
        }
        next_k++;
    }

    return cache[n - 1];  /* a(1) is at index 0 in the array */
}
