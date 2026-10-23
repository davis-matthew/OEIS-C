/*
 * A049587  Primes p such that x^55 = 2 has a solution mod p.
 * https://oeis.org/A049587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that x^55 ≡ 2 (mod p) has a
 *            solution. A prime p (including 2) satisfies the condition iff
 *            2^((p-1)/gcd(p-1,55)) ≡ 1 (mod p). This check is performed for
 *            each candidate prime until the nth such prime is found
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_u64, is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/mod_pow.h"
#include "../utils/gcd_u64.h"

static bool satisfies(uint64_t p)
{
    if (p == 2) return true;
    uint64_t g = gcd_u64(p - 1, 55ULL);
    uint64_t exp = (p - 1) / g;
    return mod_pow((seq_t)2, exp, (seq_t)p) == (seq_t)1;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    uint64_t candidate = 2;

    while (1) {
        if (candidate == 2 || candidate % 2 == 1) {
            if (is_prime_big((seq_t)candidate)) {
                if (satisfies(candidate)) {
                    found++;
                    if (found == n) {
                        return (seq_t)candidate;
                    }
                }
            }
        }
        candidate++;
    }
}
