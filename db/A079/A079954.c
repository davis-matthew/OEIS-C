/*
 * A079954  Partial sums of A030301.
 * https://oeis.org/A079954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n - 1 - (2/3)*(4^e4 - 1) - (-1)^e2*(n - 1 -
 *            2*(4^e4-1)))/2, where e4 = floor(log_4(n)), e2 =
 *            floor(log_2(n)). This implements the closed‑form description of
 *            A079954, the partial sums of A030301
 * Range:     u8 n <= 595, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
 
seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    /* Compute e2 = floor(log2(n)) and e4 = floor(log4(n)). */
    idx_t e2 = 63 - __builtin_clzll(n);          /* 0‑based index of MSB */
    idx_t e4 = e2 >> 1;                          /* floor(e2/2) */
 
    /* t = 4^e4 = 2^(2*e4)  */
    seq_t t = (seq_t)1 << (e4 << 1);
 
    /* two_thirds = (2/3)*(t-1).  Since t≡1 (mod 3), (t-1) is divisible by 3. */
    seq_t two_thirds = ((t - 1) / (seq_t)3) * (seq_t)2;
 
    if ((e2 & 1) == 0) { /* e2 even  */
        /* a = (2*t - 2 - two_thirds) / 2 */
        seq_t numerator = ((seq_t)2 * t) - (seq_t)2 - two_thirds;
        return numerator / (seq_t)2;
    } else {             /* e2 odd   */
        seq_t n_seq = (seq_t)n;
        /* a = (2*n - 2*t - two_thirds) / 2 */
        seq_t numerator = (seq_t)2 * n_seq - (seq_t)2 * t - two_thirds;
        return numerator / (seq_t)2;
    }
}
