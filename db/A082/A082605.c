/*
 * A082605  Using Euler's 6-term sequence A014556, we define the partial
 *          recurrence relation a(0)=2, a(1)=3, a(2)=5; a(k) = 2*a(k-1) - 1 -
 *          (-2)^(k-2), 3 <= k <= 5.
 * https://oeis.org/A082605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 for n=0, 3 for n=1, * a(n) = 5 for n=2, * a(n) = 2^n +
 *            1 when n is even (≥2), * a(n) = 5·2^(n−2) + 1 when n is odd (≥3)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;
    if ((n & 1) == 0) {
        /* even n: a(n) = 2^n + 1 */
        return pow2(n) + (seq_t)1;
    } else {
        /* odd n ≥ 3: a(n) = 5·2^(n−2) + 1 */
        seq_t p = pow2(n - 2);
        return p * (seq_t)5 + (seq_t)1;
    }
}
