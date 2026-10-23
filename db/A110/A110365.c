/*
 * A110365  a(1)=2, a(n+1) = a(n)*A010888(a(n)).
 * https://oeis.org/A110365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 4^{floor((n‑1)/2)+1} * 7^{floor((n‑2)/2)} for n ≥ 2, a(1)
 *            = 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the recurrence: a(1)=2, a(2)=4, a(3)=16,
   a(4)=112, a(5)=448, a(n+1)=a(n)* (4 if n+1 is odd else 7). */

#include <stdint.h>   /* for clarity; seq_t and idx_t are defined by the build */
#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    if (n == 1)
        return 2;
    /* Count how many odd and even indices from 3..n */
    idx_t oddCount   = (n - 1) / 2;          /* number of odd i >= 3 */
    idx_t evenCount  = (n - 2) / 2;          /* number of even i >= 4 */
    seq_t fourPow = pow_seq(4, oddCount + 1);   /* extra factor 4 for i=2 */
    seq_t sevenPow = pow_seq(7, evenCount);
    return fourPow * sevenPow;
}
