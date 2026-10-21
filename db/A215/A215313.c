/*
 * A215313  Primes congruent to {1, 2, 3, 4, 5} mod 19.
 * https://oeis.org/A215313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_allowed, grow_allowed_check, grow_primes_check
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes congruent to {1,2,3,4,5} mod 19 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/grow_allowed.h"
#include "../utils/grow_allowed_check.h"
#include "../utils/grow_primes_check.h"

seq_t a(idx_t n)
{
    static seq_t *allowed = NULL;
    static size_t allowed_cap = 0;
    static idx_t allowed_cnt = 0;

    static uint32_t *primes = NULL;
    static size_t primes_cap = 0;
    static idx_t primes_cnt = 0;

    static uint32_t curr = 0;   /* current candidate to test for primality */

    /* Ensure we have at least n allowed primes */
    while (allowed_cnt < n) {
        /* Start with 2 on first call */
        if (curr == 0) curr = 2;

        bool is_prime = true;

        /* Trial division using stored primes */
        for (idx_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > curr) break;
            if (curr % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* store prime in primes list */
            grow_primes_check(primes_cnt + 1, &primes_cap, &primes);
            primes[primes_cnt++] = curr;

            /* check the residue modulo 19 */
            uint32_t r = curr % 19;
            if (r >= 1 && r <= 5) {
                grow_allowed_check(allowed_cnt + 1, &allowed_cap, &allowed);
                allowed[allowed_cnt++] = (seq_t)curr;
            }
        }

        /* advance to next candidate */
        ++curr;
        while (curr % 2 == 0) ++curr;  /* skip even numbers after 2 */
    }

    return allowed[n - 1];
}
