/*
 * A166523  a(n) = 8*n - a(n-1), with n>1, a(1)=1.
 * https://oeis.org/A166523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n + 2 + 5*(-1)^n
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute for n >= 1 */
    seq_t val = (seq_t)4 * (seq_t)n + 2;
    if (n & 1) {          /* odd n: subtract 5 */
        val -= 5;
    } else {             /* even n: add 5 */
        val += 5;
    }
    return val;
}
