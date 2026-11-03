/*
 * A113484  a(n) = smallest composite integer > n and coprime to n.
 * https://oeis.org/A113484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest composite integer > n that is coprime to n
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 88 terms through n = 986 (2026-11-01)
 * Helpers:   gcd, is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_seq.h"

/* Euclidean algorithm for seq_t */

/* Simple deterministic primality test for seq_t */

seq_t a(idx_t n)
{
    seq_t np = (seq_t)n;           /* cast index to seq_t */
    seq_t m  = np + (seq_t)1;      /* start searching from n + 1 */

    while (true) {
        /* composite: m > 1 and not prime */
        if (m > (seq_t)1 && !is_prime_seq(m) && gcd(np, m) == (seq_t)1)
            return m;
        m++;
    }
}
