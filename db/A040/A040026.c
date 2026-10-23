/*
 * A040026  If n=r*2^s, r odd, then a(n)=t*r, where t is smallest (in
 *          magnitude) number such that 1=t*r+u*2^s.
 * https://oeis.org/A040026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice, look
 * Formula:   a(n) = r * t where n = r·2^s, r odd, t is the integer of minimum
 *            absolute value satisfying 1 = t·r + u·2^s
 * Range:     i8 n <= 128, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle odd n directly: a(n) = n */
    if ((n & 1) == 1)
        return (seq_t)n;

    /* split n into odd part r and exponent s of 2 */
    idx_t s = 0;
    idx_t temp = n;
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++s;
    }
    idx_t r = temp;                     /* r is odd */

    /* modulus M = 2^s (even) */
    idx_t M = ((idx_t)1) << s;

    /* extended Euclid to find inverse of r modulo M */
    idx_t a_ = r;
    idx_t b_ = M;
    __int128 u = 1;                     /* coefficients for a_ */
    __int128 v = 0;                     /* coefficients for b_ */

    while (b_ != 0) {
        idx_t q = a_ / b_;
        idx_t rem = a_ - q * b_;
        a_ = b_;
        b_ = rem;

        __int128 tmp = u - (__int128)q * v;
        u = v;
        v = tmp;
    }

    /* u is inverse of r modulo M; normalize to 0 .. M-1 */
    __int128 inv = u % (__int128)M;
    if (inv < 0)
        inv += (__int128)M;

    /* choose t with minimal absolute value */
    idx_t rem_val = (idx_t)inv;          /* 0 <= rem_val < M */
    idx_t halfM = M >> 1;
    seq_t t;
    if (rem_val > halfM) {
        t = (seq_t)(((__int128)rem_val - (__int128)M));
    } else {
        t = (seq_t)rem_val;
    }

    /* a(n) = t * r */
    return t * (seq_t)r;
}
