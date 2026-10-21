/*
 * A084021  Product of n and its 9's complement.
 * https://oeis.org/A084021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n * (10^d - 1 - n) where d is the number of decimal
 *            digits in n (a(0)=0)
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute 10^d: the smallest power of 10 greater than n. */
    seq_t pow10 = 10;          /* corresponds to 10^1 */
    idx_t temp = n;
    while (temp >= 10) {
        temp /= 10;
        pow10 *= 10;
    }

    seq_t sn   = (seq_t)n;
    seq_t bound = pow10 - 1;
    return sn * (bound - sn);
}
