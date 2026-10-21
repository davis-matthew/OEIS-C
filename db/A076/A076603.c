/*
 * A076603  a(1)=9, a(n_even)=(a(n-1)^2-1)/2; a(n_odd)=a(n-1)+1.
 * https://oeis.org/A076603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=9; a(n_even)=(a(n-1)^2-1)/2; a(n_odd)=a(n-1)+1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)          /* offset is 1; n<1 is outside the definition */
        return 0;

    seq_t value = 9;   /* a(1) */
    if (n == 1)         /* return immediately for the first term */
        return value;

    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1) == 0)       /* even index */
            value = (value * value - 1u) / 2;
        else                    /* odd index */
            value = value + 1;
    }
    return value;
}
