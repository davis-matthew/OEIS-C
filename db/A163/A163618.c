/*
 * A163618  a(2*n) = 2 * a(n). a(2*n - 1) = 2 * a(n) - 2 - (-1)^n, for all n
 *          in Z.
 * https://oeis.org/A163618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*n) = 2*a(n), a(2*n-1) = 2*a(n)-2-(-1)^n, a(0)=0, a(1)=1
 * Range:     u8 n <= 255, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t _a_rec(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    if ((n & 1) == 0) /* even: a(2*k) = 2*a(k) */
        return _a_rec(n >> 1) << 1;
    else {
        idx_t k = (n + 1) >> 1;          /* n = 2*k-1  */
        seq_t val = _a_rec(k) << 1;      /* 2*a(k)      */
        if ((k & 1) == 0)               /* k even      */
            return val - 3;             /* 2*a(k)-3    */
        else
            return val - 1;             /* 2*a(k)-1    */
    }
}

seq_t a(idx_t n)
{
    return _a_rec(n);
}
