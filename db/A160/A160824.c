/*
 * A160824  a(1)=1, a(n) = the smallest positive integer such that both a(n)
 *          and Sum_{k=1..n} a(k) have the same number of (nonleading) 0's
 *          when they are represented in binary.
 * https://oeis.org/A160824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1, a(2)=5, a(3)=1, a(4)=6. For even n ≥ 6: a(n) =
 *            2^(n/2-1). For odd n ≥ 5: a(n) = 9 * 2^((n-5)/2)
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)6;

    if ((n & 1) == 0) { /* even */
        idx_t k = n >> 1;          /* k = n/2 */
        return pow2(k - 1);        /* 2^(k-1) */
    } else {                    /* odd */
        idx_t exp = (n - 5) >> 1;  /* exp = (n-5)/2 */
        seq_t r = pow2(exp);       /* 2^exp */
        return r * (seq_t)9;       /* 9 * 2^exp */
    }
}
