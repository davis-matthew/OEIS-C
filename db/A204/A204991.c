/*
 * A204991  2^k-2^j, where (2^k,2^j) is the least pair of distinct positive
 *          powers of 2 for which n divides 2^k-2^j.
 * https://oeis.org/A204991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A204991: the smallest positive difference 2^k−2^j (k>j≥1)
   such that n | (2^k−2^j).  For n = 2^e·o (o odd), the answer is
   2^max(e,1) · (2^ord_o(2) − 1), where ord_o(2) is the multiplicative
   order of 2 modulo o.  */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not defined for n=0 but safe */

    /* separate the 2-adic factor: n = 2^e · odd, odd odd */
    idx_t t = n;
    idx_t e = 0;
    while ((t & 1) == 0) {
        t >>= 1;
        e++;
    }
    idx_t odd = t;

    /* find the multiplicative order of 2 modulo odd */
    idx_t m;
    if (odd == 1) {
        m = 1;                       /* 2^1≡1 (mod 1) */
    } else {
        idx_t val = 2 % odd;
        m = 1;
        while (val != 1) {
            val = (val * 2) % odd;
            m++;
        }
    }

    /* the exponent of the minimal power of two factor j */
    idx_t j = (e >= 1) ? e : 1;

    /* compute the result: 2^j * (2^m − 1) */
    seq_t res = pow2(j);
    seq_t diff = pow2(m);
    diff -= 1;
    res *= diff;
    return res;
}
