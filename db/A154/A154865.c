/*
 * A154865  Numbers k such that smallest digit of k is prime and the smallest
 *          digit of prime(k) is also prime.
 * https://oeis.org/A154865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime, min_digit_idx_t, min_digit_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A154865
   The function searches for integers k where the smallest decimal digit of
   k is prime (2,3,5,7) and the smallest decimal digit of the k‑th prime number
   is also prime.  It stores the results in a static array, expanding the
   array lazily so that the function works for random access or increasing
   indices. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/min_digit_idx_t.h"
#include "../utils/min_digit_uint32.h"

/* Test if a digit is one of the prime digits 2,3,5,7. */
static bool prime_digit(int d) {
    return d == 2 || d == 3 || d == 5 || d == 7;
}

/* Static cache for the sequence values. */
static seq_t *cache  = NULL;
static idx_t cache_len = 0;   /* capacity of cache */
static idx_t found     = 0;   /* number of terms already found */
static idx_t next_k    = 1;   /* next k to test */

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */

    /* Expand the cache if necessary. */
    if (n > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) exit(1);
        cache = new_cache;
        cache_len = n;
    }

    /* Generate terms until we have at least n of them. */
    while (found < n) {
        /* Check conditions for current k. */
        if (prime_digit(min_digit_idx_t(next_k))) {
            uint32_t pk = get_prime(next_k);   /* k-th prime */
            if (prime_digit(min_digit_uint32(pk))) {
                cache[found] = (seq_t)next_k;   /* store as seq_t */
                ++found;
            }
        }
        ++next_k;
    }

    return cache[n - 1];
}
