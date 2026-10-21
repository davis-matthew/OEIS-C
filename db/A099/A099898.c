/*
 * A099898  Shifts left and divides by 4 under the XOR BINOMIAL transform
 *          (A099899).
 * https://oeis.org/A099898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = SumXOR_{k=0..n-1} (C(n-1-k + [k/2], [k/2]) mod 2) *
 *            4^(k+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0) = 1 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)0;
    /* The term for each k is 4^(k+1) = 1 << (2*(k+1)) */
    for (idx_t k = 0; k < n; ++k) {
        /* Parity of binomial C(n-1-k + floor(k/2), floor(k/2)) is 1
           iff (floor(k/2) & (n-1-k)) == 0  (Lucas theorem for mod 2)   */
        idx_t r = k >> 1;                     /* floor(k/2)          */
        idx_t m_minus_r = n - 1 - k;           /* n-1-k               */
        if ((r & m_minus_r) == 0) {           /* term contributes    */
            /* shift by 2*(k+1) bits */
            idx_t shift = (k + 1) << 1;
            seq_t term = (seq_t)1 << shift;
            result ^= term;                   /* XOR sum */
        }
    }
    return result;
}
