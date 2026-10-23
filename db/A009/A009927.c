/*
 * A009927  Coordination sequence for Cr3Si, Si position.
 * https://oeis.org/A009927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (187*n*n + c1[n mod 12]*n + c0[n mod 12])/12, for n >= 3;
 *            initial values: a(0)=1, a(1)=12, a(2)=50
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int C1[12] = {
    -72, -86, -100, -54, -104, -118,
    -36, -86, -136, -54, -68, -118
};

static const int C0[12] = {
    -216, 139, 196, -81, 40, 43,
     36, 175, -56, -117, 292, 79
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)50;

    seq_t nn = (seq_t)n;
    idx_t mod = n % 12;
    int idx = (int)mod;

    seq_t num = (seq_t)187;
    num *= nn;   /* num = 187*n   */
    num *= nn;   /* num = 187*n^2 */

    /* num += C1[idx] * n */
    seq_t temp = (seq_t)C1[idx];
    temp *= nn;
    num += temp;

    /* num += C0[idx] */
    num += (seq_t)C0[idx];

    return num / (seq_t)12;
}
