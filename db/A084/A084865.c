/*
 * A084865  Primes of the form 2x^2 + 3y^2.
 * https://oeis.org/A084865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 51, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes that can be written as 2x²+3y².
   These are exactly the primes p with p ≡ 2,3,5,11 (mod 24). */

/* Note: seq_t and idx_t are provided by the build system. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */

/* Prime list (uint32_t is safe for the first 10000 such primes) */
static uint32_t *primes_arr = NULL;
static size_t   primes_cnt = 0;
static size_t   primes_cap = 0;

/* Sequence list (seq_t may be arbitrary-precision) */
static seq_t *good_arr = NULL;
static size_t good_cnt  = 0;
static size_t good_cap  = 0;

/* Next integer to test for primality */
static idx_t next_candidate = 2;

/* Ensure that good_arr contains at least n terms.  This function is
   idempotent; calling it with the same or larger n only appends more
   terms. */
static void ensure_terms(idx_t n)
{
    while (good_cnt < (size_t)n) {
        seq_t cand = (seq_t)next_candidate;
        bool is_prime = true;

        /* Skip even numbers >2 */
        if ((next_candidate > 2) && ((next_candidate & 1) == 0)) {
            is_prime = false;
        } else {
            /* Trial division by known primes up to sqrt(cand) */
            seq_t sqrt_cand = floor_sqrt(cand);
            for (size_t i = 0; i < primes_cnt; ++i) {
                seq_t p = (seq_t)primes_arr[i];
                if (p > sqrt_cand) break;
                if (cand % p == 0) {
                    is_prime = false;
                    break;
                }
            }
        }

        if (is_prime) {
            /* Append to primes_arr */
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                uint32_t *tmp = (uint32_t *)realloc(primes_arr, new_cap * sizeof(uint32_t));
                if (!tmp) exit(EXIT_FAILURE);
                primes_arr = tmp;
                primes_cap = new_cap;
            }
            primes_arr[primes_cnt++] = (uint32_t)next_candidate;

            /* Check representability condition */
            /* We accept primes with p≡2,3,5,11 (mod 24). The prime 2 is included. */
            unsigned int rem = (unsigned int)(cand % 24);
            if (next_candidate == 2 || rem == 3 || rem == 5 || rem == 11) {
                if (good_cnt == good_cap) {
                    size_t new_cap = good_cap ? good_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(good_arr, new_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    good_arr = tmp;
                    good_cap = new_cap;
                }
                good_arr[good_cnt++] = cand;
            }
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Undefined for n < 1; guard. */
    ensure_terms(n);
    return good_arr[(size_t)n - 1];
}
