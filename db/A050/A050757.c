/*
 * A050757  Primes containing no pair of consecutive equal digits.
 * https://oeis.org/A050757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime that contains no pair of consecutive equal
 *            digits * * This implementation generates primes sequentially and
 *            keeps a cache of * primes that satisfy the
 *            digit‑consecutive‑unequal property. The cache is * retained
 *            between calls, so each prime is examined at most once. The *
 *            hel...
 * Range:     u8 n <= 46, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   grow_capacity, no_consecutive_equal_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper header providing the bitmask test used below */
#include "../utils/no_consecutive_equal_digits.h"
#include "../utils/grow_capacity.h"

/* The public sequence function */
seq_t a(idx_t n)
{
    /* Static state retained between calls */
    static bool initialized = false;

    /* All prime candidates that we have discovered so far (just the value, not the property) */
    static uint64_t *primes_buf = NULL;
    static size_t primes_len = 0, primes_cap = 0;

    /* Primes that satisfy the “no consecutive equal digits” property */
    static uint64_t *good_buf = NULL;
    static size_t good_len = 0, good_cap = 0;

    /* The next odd number to test as a prime candidate */
    static uint64_t next_candidate = 3;

    /* One‑time initialization */
    if (!initialized) {
        grow_capacity(&primes_cap);
        primes_buf = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes_len = 1;
        primes_buf[0] = 2;                /* 2 is the first prime */

        grow_capacity(&good_cap);
        good_buf = (uint64_t *)malloc(good_cap * sizeof(uint64_t));
        good_len = 1;
        good_buf[0] = 2;                  /* 2 has no consecutive equal digits */

        initialized = true;
    }

    /* Ensure the buffer can hold the requested prime index */
    while (good_len < (size_t)n) {
        /* Test next_candidate for primality */
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes_buf[i];
            if ((unsigned __int128)p * p > next_candidate)
                break;
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store the new prime */
            if (primes_len == primes_cap) {
                grow_capacity(&primes_cap);
                primes_buf = (uint64_t *)realloc(primes_buf, primes_cap * sizeof(uint64_t));
            }
            primes_buf[primes_len++] = next_candidate;

            /* Check the digit property and store if it matches */
            if (no_consecutive_equal_digits((seq_t)next_candidate)) {
                if (good_len == good_cap) {
                    grow_capacity(&good_cap);
                    good_buf = (uint64_t *)realloc(good_buf, good_cap * sizeof(uint64_t));
                }
                good_buf[good_len++] = next_candidate;
            }
        }

        next_candidate += 2;   /* Only odd numbers need be tested */
    }

    /* Return the n‑th element (1‑based index) in the good list */
    return (seq_t)good_buf[n - 1];
}
