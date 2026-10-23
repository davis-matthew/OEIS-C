/*
 * A020649  Least quadratic nonresidue modulo n (with n >= 3).
 * https://oeis.org/A020649
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4701, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: find the least quadratic nonresidue modulo n for n ≥ 3.
 *
 * The routine works as follows:
 *   - Factor n into 2^exp2 * ∏pi.
 *   - For each prime candidate q starting at 2:
 *       * If q==2 and exp2≥2, 2 is a nonresidue modulo 2^exp2 and hence modulo n.
 *       * If q is odd:
 *           – Check the 2–factor: if exp2==2 and q%4≠1 or exp2≥3 and q%8≠1, q is a nonresidue.
 *           – For every odd prime factor pi, evaluate the Legendre symbol (q|pi)
 *             via Euler’s criterion: q^((pi−1)/2) ≡ pi−1 (mod pi) means nonresidue.
 *       * The first q that satisfies one of the above is returned.
 *
 * All heavy work is done by a trial factorisation (O(√n)) and
 * small modular exponentiations using the provided "mod_pow" utility.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mod_pow.h"

static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* Factorise n into 2^exp2 and distinct odd primes.
 * primes[] receives the odd primes, up to maxcnt entries.
 * Returns the count of odd primes, and writes exp2. */
static size_t factor_uint64(uint64_t n, uint64_t *primes, size_t maxcnt,
                            uint64_t *exp2)
{
    size_t cnt = 0;
    *exp2 = 0;
    while ((n & 1ULL) == 0ULL) { ++(*exp2); n >>= 1; }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            if (cnt < maxcnt) primes[cnt++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) {
        if (cnt < maxcnt) primes[cnt++] = n;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥3 by the contract. */
    uint64_t twos_exp;
    uint64_t primes[64];
    size_t prime_cnt = factor_uint64((uint64_t)n, primes, 64, &twos_exp);

    for (uint64_t q = 2; ; ++q) {
        if (!is_prime_uint64(q)) continue;

        /* Handle the 2‑power component first. */
        if (q == 2) {
            if (twos_exp >= 2) return (seq_t)2;
        } else {
            /* Odd q: check solvability modulo 2^e. */
            if (twos_exp == 2 && (q % 4) != 1) return (seq_t)q;
            if (twos_exp >= 3 && (q % 8) != 1) return (seq_t)q;
        }

        /* Check all odd prime factors of n. */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            /* Legendre symbol via Euler's criterion. */
            seq_t r = mod_pow((seq_t)q,
                              (uint64_t)((p - 1) / 2),
                              (seq_t)p);
            if (r == (seq_t)(p - 1)) return (seq_t)q;  /* nonresidue mod p */
        }
    }
}
