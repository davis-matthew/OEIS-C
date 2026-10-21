/*
 * A066787  a(n) = gcd(2^n + 1, n^2 + 1).
 * https://oeis.org/A066787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(2^n + 1, n^2 + 1)
 * Range:     u8 n <= 17, u16 n <= 260, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* n^2 + 1 */
    uint64_t tmp = (uint64_t)n * (uint64_t)n + (uint64_t)1;
    seq_t m = (seq_t)tmp;                   /* modulus for the power */

    /* 2^n mod (n^2 + 1) */
    seq_t pow_val = mod_pow((seq_t)2, n, m);

    /* (2^n + 1) mod (n^2 + 1) */
    seq_t plus = pow_val + (seq_t)1;
    if (plus == m)                        /* wrap‑around when 2^n + 1 == n^2 + 1 */
        plus = (seq_t)0;

    return gcd(plus, m);
}
