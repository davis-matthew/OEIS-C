/*
 * A107749  OrdinaryUnitarySigma(n): If n = Product p_i^r_i then OUSigma(n) =
 *          Sigma(2^r_1)*UnitarySigma(n/2^r_1).
 * https://oeis.org/A107749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = (2^{v2(n)+1}-1) * Π_{p odd, p^e‖n} (p^e + 1) where v2(n)
 *            is the exponent of 2 in the prime power decomposition of n
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_plus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow_plus_one.h"

/* Maximum number of primes below 1 000 000 – sufficient for factoring
   numbers up to 10 000 though this implementation works for any 64‑bit n. */
#define PRIME_CAP 80000

static uint32_t primes[PRIME_CAP];
static size_t prime_cnt = 0;

/* Simple sieve that fills the global primes table.  It is run only once
   on the first call of a(). */
static void init_primes(void)
{
    const uint32_t limit = 1000000U;
    bool *is_composite = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_composite) { /* a very unlikely allocation failure – abort */
        exit(1);
    }

    for (uint32_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            if (prime_cnt < PRIME_CAP) {
                primes[prime_cnt++] = i;
            }
            if ((uint64_t)i * i <= limit) {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i) {
                    is_composite[(size_t)j] = true;
                }
            }
        }
    }
    free(is_composite);
}

seq_t a(idx_t n)
{
    /* Lazy static initialization of primes. */
    static bool initialized = false;
    if (!initialized) {
        init_primes();
        initialized = true;
    }

    /* Handle the n == 0 case defensively – the sequence is defined
       starting at n = 1. */
    if (n == 0) {
        return 0;
    }

    seq_t result = 1;
    uint64_t tmp = (uint64_t)n;

    /* Separate out the exponent of 2. */
    uint64_t e2 = 0;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        ++e2;
    }
    if (e2 > 0) {
        seq_t sigma2 = 1;
        for (uint64_t i = 0; i < e2 + 1; ++i) {
            sigma2 *= 2;
        }
        sigma2 -= 1;                /* sigma(2^e2) = 2^{e2+1} - 1 */
        result *= sigma2;
    }

    /* Factor the odd part of n. */
    uint64_t val = tmp;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= val; ++i) {
        uint64_t p = primes[i];
        if (val % p == 0) {
            uint64_t exp = 0;
            while (val % p == 0) {
                val /= p;
                ++exp;
            }
            result *= pow_plus_one(p, exp);
        }
    }
    if (val > 1) {                      /* val is a remaining prime factor */
        result *= (seq_t)val + 1;
    }

    return result;
}
