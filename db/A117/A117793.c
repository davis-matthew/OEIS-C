/*
 * A117793  Pentagonal numbers divisible by 5.
 * https://oeis.org/A117793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2*k+1) = 5*k*(15*k-1)/2; a(2*k+2) = 5*(5*k+2)*(3*k+1)/2
 * Range:     u8 n <= 6, u16 n <= 84, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1; undefined behaviour for 0 */
        return 0;
    if (n & 1) {           /* odd n: n = 2*k+1   => k = (n-1)/2           */
        seq_t k = (seq_t)(n - 1) / 2;
        return (seq_t)5 * k * ((seq_t)15 * k - 1) / 2;
    } else {               /* even n: n = 2*k+2  => k = (n-2)/2          */
        seq_t k = (seq_t)(n - 2) / 2;
        return (seq_t)5 * ((seq_t)5 * k + 2) * ((seq_t)3 * k + 1) / 2;
    }
}
