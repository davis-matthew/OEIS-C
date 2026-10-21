/*
 * A052276  Nonnegative numbers of the form n^3 (+/-) 3, n >= 0.
 * https://oeis.org/A052276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n/2)^3 + 3*(-1)^n for n > 1, with a(1) = 3
 * Range:     u8 n <= 12, u16 n <= 80, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;

    /* ceil(n/2) for unsigned integers */
    seq_t k = (n + 1) / 2;

    /* compute k^3 using integer arithmetic */
    seq_t k3 = k * k * k;

    /* for odd n subtract 3, for even n add 3 */
    return (n & 1) ? (k3 - (seq_t)3) : (k3 + (seq_t)3);
}
