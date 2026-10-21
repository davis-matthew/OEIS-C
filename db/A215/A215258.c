/*
 * A215258  Smallest number h such that (2n+1)*h is a repunit (A002275), or 0
 *          if no such h exists.
 * https://oeis.org/A215258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest h such that (2n+1)*h is a repunit
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If no such h exists, returns 0.                             */
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* d = 2n+1, must be coprime to 10 */
    seq_t d = ((seq_t)n << 1) + 1;
    if (gcd(d, (seq_t)10) != (seq_t)1)
        return (seq_t)0;

    /* m = 9*d.  m is small (<= 180009 for n <= 10000). */
    seq_t m = (seq_t)9 * d;
    uint64_t mod = (uint64_t)m;           /* safe cast: m < 2^63 */

    /* Find the multiplicative order k of 10 modulo m   */
    uint64_t t = 10 % mod;
    idx_t k = 1;
    while (t != 1) {
        t = (t * 10) % mod;
        ++k;
    }

    /* Compute h by long division of the repunit R_k = 11..1 (k times) by d   */
    uint64_t dd = (uint64_t)d;   /* d fits in 64‑bit */
    uint64_t rem = 0;
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < k; ++i) {
        rem = rem * 10 + 1;                /* next digit of the repunit */
        uint64_t q = rem / dd;             /* digit of the quotient (0..9) */
        rem = rem % dd;
        result = result * (seq_t)10 + (seq_t)q;
    }
    return result;
}
