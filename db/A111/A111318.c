/*
 * A111318  Numbers k such that 4 divides prime(1) + ... + prime(k).
 * https://oeis.org/A111318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that the sum of the first k
 *            primes is divisible by 4
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Static state for incremental computation */
    static size_t k_len  = 0;           /* number of indices found so far   */
    static size_t k_cap  = 0;           /* capacity of k_indices array      */
    static seq_t  *k_indices = NULL;    /* array of k-values (prime indices)*/

    static idx_t   prime_idx = 1;       /* next prime index to process      */
    static unsigned sum_mod4 = 0;       /* running sum of primes modulo 4   */

    /* Ensure there is room for at least n indices in the array */
    if (n > k_cap) {
        size_t new_cap = (k_cap == 0) ? 64 : k_cap * 2;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(k_indices, new_cap * sizeof(seq_t));
        if (!tmp) abort();                       /* allocation failure – hard stop */
        k_indices = tmp;
        k_cap = new_cap;
    }

    /* Generate further primes until we have at least n k-values */
    while (k_len < (size_t)n) {
        seq_t prime_val = get_prime((uint64_t)prime_idx);  /* prime (as seq_t) */
        /* Convert to uint64_t for the modulo 4 operation – primes here are tiny */
        uint64_t val = (uint64_t)prime_val;
        sum_mod4 = (sum_mod4 + (unsigned)(val & 3u)) & 3u;   /* keep only modulo 4 */

        if (sum_mod4 == 0) {
            /* Store the prime index (k) as seq_t */
            k_indices[k_len++] = (seq_t)(prime_idx);
        }
        ++prime_idx;
    }

    /* Return the requested k-value (prime index) */
    return k_indices[(size_t)(n - 1)];
}
