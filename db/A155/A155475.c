/*
 * A155475  Even composites n such that smallest digit of n < smallest digit
 *          of n-th prime.
 * https://oeis.org/A155475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   ensure_capacity, min_digit_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth even composite such that the smallest decimal
 * digit of the number is less than the smallest decimal digit of the
 * nth prime.  The sequence is defined by OEIS A155475. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"
#include "../utils/min_digit_seq.h"

/* Dynamic list of primes used to obtain the nth prime. */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap = 0;
static uint64_t prime_next = 2;   /* next number to test for primality */

/* Ensure prime_list has at least `need` primes.  Generates primes
 * incrementally.
 */
static seq_t nth_prime(idx_t idx)
{
    while (prime_count < idx) {
        bool composite = false;
        if (prime_next > 2 && (prime_next & 1) == 0) {
            composite = true;               /* even > 2 -> composite */
        } else {
            /* trial division by existing primes */
            for (size_t i = 0; i < prime_count; ++i) {
                uint64_t p = prime_list[i];
                if (p > prime_next / p) break;   /* p*p > prime_next */
                if (prime_next % p == 0) {
                    composite = true;
                    break;
                }
            }
        }
        if (!composite) {
            ensure_capacity(prime_count + 1, &prime_cap,
                            (void**)&prime_list, sizeof(uint64_t));
            prime_list[prime_count++] = prime_next;
        }
        ++prime_next;
    }
    return (seq_t)prime_list[idx - 1];
}

/* Cache of computed terms of the sequence. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Next candidate even number to examine.  Starts at 4 and is
 * increased by 2 after each inspection, regardless of whether
 * the candidate satisfies the condition.
 */
static idx_t next_candidate = 4;

/* Main sequence function. */
seq_t a(idx_t n)
{
    while (cache_len < n) {
        /* Find the next even composite candidate. */
        /* Even numbers >2 are always composite, so we simply
         * examine every even number starting from 4.
         */
        /* Test the condition for the current candidate. */
        seq_t cand = (seq_t)next_candidate;
        uint8_t cand_min = min_digit_seq(cand);

        /* Obtain the smallest digit of the nth prime, where
         * the index is the candidate number itself.
         */
        seq_t prime_val = nth_prime(next_candidate);
        uint8_t prime_min = min_digit_seq(prime_val);

        if (cand_min < prime_min) {
            /* Store the candidate in the cache. */
            ensure_capacity(cache_len + 1, &cache_cap,
                            (void**)&cache, sizeof(seq_t));
            cache[cache_len++] = cand;
        }
        /* Move to the next even number. */
        next_candidate += 2;
    }
    return cache[n - 1];
}
