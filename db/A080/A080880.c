/*
 * A080880  a(n)*a(n+3) - a(n+1)*a(n+2) = 2^n, given a(0)=1, a(1)=2, a(2)=2.
 * https://oeis.org/A080880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = (4^n + 2) / 3, a(2n+1) = 4^n + 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 4^k where k = floor(n/2) */
    idx_t k = n >> 1;              /* k = n/2 */
    seq_t pow4 = 1;

    for (idx_t i = 0; i < k; ++i)
        pow4 *= (seq_t)4;

    if ((n & 1) == 0) {           /* even n : a(2k) = (4^k + 2) / 3 */
        return (pow4 + (seq_t)2) / (seq_t)3;
    } else {                      /* odd n  : a(2k+1) = 4^k + 1 */
        return pow4 + (seq_t)1;
    }
}
