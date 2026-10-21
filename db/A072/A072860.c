/*
 * A072860  Highest power of 3 dividing the period length of 1/prime(n) =
 *          A002371(n).
 * https://oeis.org/A072860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = highest power of 3 dividing the multiplicative order of
 *            10 mod prime(n) * (0 if prime(n) is 2 or 5)
 * Range:     u8 n <= 57, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Includes for helper functions and integer types. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime.h"        /* uint32_t get_prime(idx_t k); */
#include "../utils/mod_pow.h"          /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

/*  Compute a(n). */
seq_t a(idx_t n)
{
    /* Special cases: p = 2 or p = 5 give terminating decimals, period length 0. */
    uint32_t p = get_prime(n);          /* nth prime */
    if (p == 2 || p == 5) {
        return 0;
    }

    /*  Factor p-1: collect distinct prime factors. */
    uint64_t m = (uint64_t)p - 1ULL;
    uint64_t primes[64];
    int prime_cnt = 0;

    for (uint64_t q = 2; q * q <= m; ++q) {
        if (m % q == 0) {
            primes[prime_cnt++] = q;
            while (m % q == 0) {
                m /= q;
            }
        }
    }
    if (m > 1) {
        primes[prime_cnt++] = m;
    }

    /*  Compute the multiplicative order of 10 modulo p. */
    uint64_t order = (uint64_t)p - 1ULL;
    for (int i = 0; i < prime_cnt; ++i) {
        uint64_t f = primes[i];
        while (order % f == 0) {
            uint64_t candidate = order / f;
            if (mod_pow((seq_t)10, candidate, (seq_t)p) == (seq_t)1) {
                order = candidate;
            } else {
                break;
            }
        }
    }

    /*  Extract the highest power of 3 dividing that order. */
    seq_t power = 1;
    while (order % 3 == 0) {
        order /= 3;
        power *= 3;
    }

    return power;
}
