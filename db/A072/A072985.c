/*
 * A072985  Coefficient of the highest power of q in the expansion of nu(0)=1,
 *          nu(1)=b and for n >= 2, nu(n) = b*nu(n-1) + lambda*(n-1)_q*nu(n-2)
 *          with (b,lambda)=(2,3), where (n)_q = (1+q+...+q^(n-1)) and q is a
 *          root of unity.
 * https://oeis.org/A072985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1, 2, 7 for n = 0, 1, 2; * for n ≥ 3, a(n) = 3·a(n-2).
 *            Equivalently, * a(2k) = 7·3^(k-1) (k≥1), a(2k+1) = 2·3^k (k≥0)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 79, u128 n <= 159,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow3(idx_t exp)
{
    seq_t base = (seq_t)3;
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1ULL)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;
    if (n == (idx_t)2) return (seq_t)7;

    if ((n & (idx_t)1ULL) == 0) {            /* even n */
        idx_t k = n >> 1;                    /* k = n/2, k ≥ 1 */
        seq_t res = pow3(k - 1);
        res *= (seq_t)7;
        return res;
    } else {                                 /* odd n */
        idx_t k = n >> 1;                    /* k = n/2 (floor) */
        seq_t res = pow3(k);
        res *= (seq_t)2;
        return res;
    }
}
