/*
 * A142152  Primes congruent to 21 mod 38.
 * https://oeis.org/A142152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 21 (mod 38)
 * Range:     u8 n <= 4, u16 n <= 360, u32 n <= 237, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by incrementally testing candidates of the
   form 38*k + 21.  The test uses the `is_prime_big` routine provided
   by the utilities library.  The result is memoised so that calls out
   of order are fast. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

static seq_t *prime_list = NULL;  /* stored primes */
static size_t prime_count = 0;    /* how many were found */
static size_t prime_cap   = 0;    /* capacity of prime_list */
static idx_t   next_k      = 1;   /* next k to test (1 → first candidate 59) */

/* Grow the array so that one more prime can be stored */
static void ensure_capacity(void)
{
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_list = (seq_t *)realloc(NULL, prime_cap * sizeof(seq_t));
    } else if (prime_count >= prime_cap) {
        prime_cap <<= 1;  /* double */
        prime_list = (seq_t *)realloc(prime_list,
                                      prime_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, return 0 as a safe value */

    /* Grow the memoised list until we have at least n primes */
    while (prime_count < n) {
        ensure_capacity();

        /* Form candidate 38*k + 21 (all arithmetic in idx_t to avoid overflow) */
        idx_t cand_idx = (idx_t)38 * next_k + 21;
        seq_t candidate = (seq_t)cand_idx;

        if (is_prime_big(candidate)) {
            prime_list[prime_count++] = candidate;
        }
        ++next_k;
    }

    return prime_list[n - 1];
}
