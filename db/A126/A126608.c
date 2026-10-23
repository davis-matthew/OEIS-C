/*
 * A126608  Smallest odd prime q such that pq+2 is prime, where p is the n-th
 *          odd prime.
 * https://oeis.org/A126608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the smallest odd prime q such that p*q+2 is prime, where
 *            p is the nth odd prime. No closed form is known; the sequence is
 *            computed by searching over odd primes q until the condition
 *            holds
 * Range:     u8 n <= 11, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/is_prime_big.h"/* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    /* The nth odd prime is the (n+1)-th prime overall (excluding 2). */
    uint32_t p = get_prime(n + 1);

    for (idx_t q_idx = 2; ; ++q_idx) {
        uint32_t q = get_prime(q_idx);          /* start at 3 (index 2) */
        uint64_t cand64 = (uint64_t)p * q + 2;   /* compute p*q+2 safely */
        seq_t cand = (seq_t)cand64;              /* cast to the target type */
        if (is_prime_big(cand))
            return (seq_t)q;                    /* smallest odd prime found */
    }
}
