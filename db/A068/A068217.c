/*
 * A068217  Denominators of coefficients in 3*log(sqrt(1+x)) power series.
 * https://oeis.org/A068217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = (n % 3 == 0) ? 2*n/3 : 2*n
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined – return 0 for safety. */
    if (n == 0) return (seq_t)0;

    seq_t val = (seq_t)n;               /* cast to seq_t for arithmetic */
    if (n % 3 == 0) {
        val = (val * (seq_t)2) / (seq_t)3;
    } else {
        val *= (seq_t)2;
    }
    return val;
}
