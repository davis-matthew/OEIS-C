/*
 * A049588  Primes p such that x^56 = 2 has a solution mod p.
 * https://oeis.org/A049588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A049588: Primes p for which x^56 = 2 has a solution mod p.
 *
 *  Computation: For a prime p>2, compute d = gcd(56,p-1).
 *  Let e = (p-1)/d. Then 2^e ≡ 1 (mod p) means
 *  that 2 is a 56‑th power residue modulo p, hence the congruence has a
 *  solution.  The function iterates over primes, testing this criterion.
 *  2 itself is always a solution (0^56 ≡ 2 mod 2).
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for offset < 1 */
    if (n == 1) return (seq_t)2;   /* first term is 2 */

    idx_t count = 1;               /* prime 2 already counted */
    uint64_t candidate = 3;        /* start with odd numbers */

    while (true) {
        if (is_prime((idx_t)candidate)) {
            uint64_t d = gcd64(56ULL, candidate - 1);
            uint64_t exp = (candidate - 1) / d;
            seq_t val = mod_pow((seq_t)2, exp, (seq_t)candidate);
            if (val == (seq_t)1) {          /* 2 is a 56‑th power residue */
                ++count;
                if (count == n)
                    return (seq_t)candidate;
            }
        }
        candidate += 2;               /* only odd candidates */
    }
}
