/*
 * A054710  Number of powers of 10 mod n.
 * https://oeis.org/A054710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   For n≥1, let a = v₂(n), b = v₅(n), t = max(a,b) and n' = n /
 *            (2ᵃ·5ᵇ). Then a(n) = t + 1 if n' = 1 a(n) = t + ord_{n'}(10)
 *            otherwise, where ord_{m}(10) is the multiplicative order of 10
 *            modulo m
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not used, safety */

    /* Count powers of 2 */
    uint64_t un = (uint64_t)n;
    uint64_t v2 = 0, v5 = 0;
    while ((un & 1ULL) == 0 && un > 0) {
        v2++;
        un >>= 1;
    }

    /* Count powers of 5 */
    while (un % 5ULL == 0 && un > 0) {
        v5++;
        un /= 5ULL;
    }

    uint64_t t = v2 > v5 ? v2 : v5;   /* t = max(v2, v5) */

    if (un == 1) {                    /* n contains only 2ⁿ·5ⁿ */
        return (seq_t)(t + 1);
    }

    /* Now un = n' is coprime to 10. Compute φ(n') */
    uint64_t m = un;
    seq_t phi = 1;
    uint64_t temp = m;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            uint64_t pe   = 1;
            while (temp % p == 0) {
                temp /= p;
                exp++;
                if (exp > 1) pe *= p;
            }
            seq_t term = (seq_t)(p - 1) * (seq_t)pe;
            phi *= term;
        }
    }
    if (temp > 1) {                    /* remaining prime factor */
        seq_t term = (seq_t)(temp - 1);
        phi *= term;
    }

    /* φ fits into uint64_t because m ≤ 2⁶⁴–1 */
    uint64_t phi_val = (uint64_t)phi;

    /* Factor φ to get its distinct prime divisors */
    uint64_t phi_tmp = phi_val;
    uint64_t primes[64];
    int pcnt = 0;
    for (uint64_t p = 2; p * p <= phi_tmp; ++p) {
        if (phi_tmp % p == 0) {
            primes[pcnt++] = p;
            while (phi_tmp % p == 0) phi_tmp /= p;
        }
    }
    if (phi_tmp > 1) primes[pcnt++] = phi_tmp;

    /* Compute multiplicative order of 10 modulo m */
    seq_t ord = phi;
    for (int i = 0; i < pcnt; ++i) {
        uint64_t q = primes[i];
        while (ord % q == 0 &&
               mod_pow(10, (idx_t)(ord / q), (seq_t)m) == 1) {
            ord /= q;
        }
    }

    return (seq_t)t + ord;           /* final value a(n) */
}
