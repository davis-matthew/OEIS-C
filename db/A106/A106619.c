/*
 * A106619  a(n) = numerator of n/(n+18).
 * https://oeis.org/A106619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac, mult
 * Formula:   a(n) = n/gcd(n,18)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* gcd(n,18) depends only on n mod 18 */
    static const seq_t GCD_TABLE[18] = {
        (seq_t)18, (seq_t)1, (seq_t)2, (seq_t)3,
        (seq_t)2,  (seq_t)1, (seq_t)6, (seq_t)1,
        (seq_t)2,  (seq_t)9, (seq_t)2, (seq_t)1,
        (seq_t)6,  (seq_t)1, (seq_t)2, (seq_t)3,
        (seq_t)2,  (seq_t)1
    };

    seq_t N   = (seq_t)n;
    seq_t rem = N % (seq_t)18;                 /* 0 <= rem < 18 */
    seq_t g   = GCD_TABLE[(size_t)rem];
    return N / g;
}
