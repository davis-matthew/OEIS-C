/*
 * A191900  Number of compositions of odd natural numbers into 7 parts <=n.
 * https://oeis.org/A191900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n + 1)^7 - (1 + (-1)^n)/2)/2
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 22, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n + 1;          /* m = n + 1 */
    seq_t pow = 1;                   /* pow = (n+1)^7 */
    for (idx_t i = 0; i < 7; ++i) {
        pow *= m;
    }

    /* part = (1 + (-1)^n)/2, which equals 1 for even n and 0 for odd n */
    seq_t parityTerm = (n & 1) ? (seq_t)-1 : (seq_t)1;
    seq_t part = ((seq_t)1 + parityTerm) / 2;

    return (pow - part) / 2;
}
