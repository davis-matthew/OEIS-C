/*
 * A216735  Primes p such that x^8 = -2 has no solution mod p.
 * https://oeis.org/A216735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that x^8 ≡ -2 (mod p) has no solution
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* not used but defined */
    idx_t count = 0;
    uint64_t p = 5;                            /* first eligible prime */

    while (count < n) {
        seq_t p_seq = (seq_t)p;
        seq_t d_seq = gcd((seq_t)8, p_seq - 1);
        uint64_t d  = (uint64_t)d_seq;
        uint64_t e  = (p - 1) / d;             /* exponent (p‑1)/gcd(8,p‑1) */
        seq_t base = p_seq - 2;                /* -2 mod p */
        seq_t pow_res = mod_pow(base, e, p_seq);

        if (pow_res != 1) {                    /* no 8th root of -2 */
            ++count;
            if (count == n) return p_seq;
        }
        p = next_prime(p);                     /* next candidate prime */
    }
    return 0; /* unreachable */
}
