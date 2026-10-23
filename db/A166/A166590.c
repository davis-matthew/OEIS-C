/*
 * A166590  Totally multiplicative sequence with a(p) = p+2 for prime p.
 * https://oeis.org/A166590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} (p+2)^e (multiplicative, a(p^e) = (p+2)^e)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Computes the nth term of OEIS sequence A166590. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not needed for OEIS offset 1, but safe */
    if (n == 1) return 1;

    /* Pre‑computed primes up to 10000 (covers all factors of n≤10000). */
    const uint32_t MAXLIMIT = 10000;          /* sqrt(10000^2) */
    static bool primes_ready = false;
    static uint32_t primes[1229];             /* 1229 primes ≤ 10000 */
    static size_t prime_cnt = 0;

    if (!primes_ready) {
        bool sieve[MAXLIMIT + 1];
        for (uint32_t i = 0; i <= MAXLIMIT; ++i) sieve[i] = true;
        sieve[0] = sieve[1] = false;
        for (uint32_t p = 2; p * p <= MAXLIMIT; ++p)
            if (sieve[p])
                for (uint32_t j = p * p; j <= MAXLIMIT; j += p)
                    sieve[j] = false;

        for (uint32_t i = 2; i <= MAXLIMIT; ++i)
            if (sieve[i])
                primes[prime_cnt++] = i;

        primes_ready = true;
    }

    seq_t result = 1;
    uint64_t tmp = n;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            size_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            seq_t base = (seq_t)p + 2;
            seq_t power = 1;
            for (size_t e = 0; e < exp; ++e) power *= base;
            result *= power;
        }
    }
    if (tmp > 1) {                  /* remaining prime factor */
        seq_t base = (seq_t)tmp + 2;
        result *= base;
    }

    return result;
}
