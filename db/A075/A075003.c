/*
 * A075003  Floor[ concatenation of n+1 and n divided by 2 ].
 * https://oeis.org/A075003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor( concat(n+1 , n) / 2 ) where concat(x , y) means
 *            the decimal concatenation of x followed by y. Example: n = 10 →
 *            concat(11,10) = 1110 → 1110/2 = 555
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 10^d where d = number of decimal digits of n (n ≥ 0)   */
    seq_t pow10 = 10;                         /* 10^1 for single digit n */
    idx_t tmp = n;
    if (tmp == 0) {                          /* n = 0 → pow10 = 10 */
        pow10 = 10;
    } else {
        /* for n > 0, raise 10 to the power of the number of digits of n */
        while (tmp >= 10) {
            pow10 *= 10;
            tmp /= 10;
        }
    }

    seq_t n_plus1 = (seq_t)(n + 1);
    seq_t s = n_plus1 * pow10 + (seq_t)n;     /* concatenated number */
    return s / 2;                             /* floor division by 2 */
}
