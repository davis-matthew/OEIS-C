/*
 * A108346  a(n) is the least positive number k such that P2(2n+1,q) divides
 *          Q(k,q), where both P2 and Q are polynomials in GF(2) and P2(n,q)
 *          is the n-th binary polynomial, i.e., P2(n,q) = Sum_{i>=0}
 *          b(i)*q^i, with n = Sum_{i>=0} b(i)*2^i; and Q(m,q) is 1 + q^m.
 * https://oeis.org/A108346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least positive k such that the binary polynomial
 *            P₂(2n+1, q) divides 1 + qᵏ over GF(2). Equivalently, k is the
 *            multiplicative order of the indeterminate q modulo P₂(2n+1)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   bitlen_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bitlen_uint128.h"

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0) return (seq_t)1;

    /* 2n + 1  →  binary polynomial P₂(2n+1) */
    unsigned __int128 m   = ((unsigned __int128)n << 1) + 1;
    unsigned __int128 mod = m;                     /* polynomial bits */

    /* P₂(1) = 1 ⇒ order always 1 */
    if (mod == 1) return (seq_t)1;

    unsigned deg = bitlen_uint128(mod) - 1;         /* highest degree */

    unsigned __int128 curr = 2;                     /* the polynomial q (x) */
    /* initial reduction: ensure degree < deg */
    if (curr & ((unsigned __int128)1 << deg)) curr ^= mod;

    for (idx_t k = 1;; ++k) {
        if (curr == 1) return (seq_t)k;            /* found the order */

        curr <<= 1;                                /* multiply by x */

        if (curr & ((unsigned __int128)1 << deg)) /* reduction */
            curr ^= mod;

        if (deg < 128)                                 /* keep only low bits */
            curr &= (((unsigned __int128)1 << deg) - 1);
    }
}
