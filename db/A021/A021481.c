/*
 * A021481  Decimal expansion of 1/477.
 * https://oeis.org/A021481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor((10^(n+1) / 477)) mod 10
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* nth digit of the decimal expansion of 1/477 */

seq_t a(idx_t n)
{
    seq_t rem = (seq_t)1;
    idx_t i;
    unsigned int digit = 0;

    for (i = 0; i <= n; ++i)
    {
        digit = (unsigned int)((rem * (seq_t)10) / (seq_t)477);
        rem   = (rem * (seq_t)10) % (seq_t)477;
    }
    return (seq_t)digit;
}
