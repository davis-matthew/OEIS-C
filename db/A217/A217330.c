/*
 * A217330  The number of integer solutions to the equation x1 + x2 + x3 + x4
 *          = n, with xi >= 0, and with x2 + x3 divisible by 3.
 * https://oeis.org/A217330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 103, u32 n <= 3797, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A217330: number of non‑negative integer solutions to
 *     x1 + x2 + x3 + x4 = n
 * with the additional condition that x2 + x3 is divisible by 3.
 *
 * Closed form (taken from the OEIS entry):
 *   let n = 3k, 3k+1, 3k+2
 *   a(3k)   = (k+1)*(3*k^2+3*k+2)/2
 *   a(3k+1) = (k+1)*(3*k^2+6*k+4)/2
 *   a(3k+2) = 3*(k+1)^2*(k+2)/2
 *
 * The implementation casts indices to seq_t before arithmetic to avoid
 * accidental overflow, as required.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 3;
    idx_t r = n % 3;
    seq_t K = (seq_t)k;               /* cast index to seq_t */
    seq_t res;

    if (r == 0) {
        /* a(3k) = (k+1)*(3*k^2+3*k+2)/2 */
        res = (K + 1) *
              (((seq_t)3 * K * K) + ((seq_t)3 * K) + (seq_t)2) /
              (seq_t)2;
    } else if (r == 1) {
        /* a(3k+1) = (k+1)*(3*k^2+6*k+4)/2 */
        res = (K + 1) *
              (((seq_t)3 * K * K) + ((seq_t)6 * K) + (seq_t)4) /
              (seq_t)2;
    } else { /* r == 2 */
        /* a(3k+2) = 3*(k+1)^2*(k+2)/2 */
        res = (seq_t)3 * (K + 1) * (K + 1) * (K + 2) / (seq_t)2;
    }

    return res;
}
