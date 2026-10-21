/*
 * A230766  Numbers with more than one prime factor and, in the ordered
 *          factorization, the exponent never decreases when read from left to
 *          right.
 * https://oeis.org/A230766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A230766: numbers whose ordered prime‐exponent sequence
 * is non‑decreasing and that have at least two distinct prime factors.
 * This implementation searches successive integers and tests each
 * for the required property. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

/* Check whether x has at least two prime factors and the exponents
 * in its prime factorization are non‑decreasing when primes ascend. */
static bool valid_number(uint64_t x, uint64_t *primes, size_t primes_cnt)
{
    uint64_t tmp = x;
    uint64_t prev_exp = 0;
    size_t fac_cnt = 0;

    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (exp < prev_exp) return false;
            prev_exp = exp;
            ++fac_cnt;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        if (1 < prev_exp) return false;
        ++fac_cnt;
    }
    return fac_cnt >= 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for zero index */

    /* Prime table once per program. 1 000 000 is ample for n≈10 000. */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    if (primes == NULL) {
        build_primes_upto(1000000, &primes, &primes_cnt);
    }

    idx_t found = 0;
    uint64_t candidate = 2;

    while (true) {
        if (valid_number(candidate, primes, primes_cnt)) {
            ++found;
            if (found == n) return (seq_t)candidate;
        }
        ++candidate;
    }
}
