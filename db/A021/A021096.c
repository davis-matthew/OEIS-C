/*
 * A021096  Decimal expansion of 1/92.
 * https://oeis.org/A021096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0)=0; for n≥1, a(n) = floor(10^(n+1)/92) mod 10 (i.e., the
 *            (n+1)-th decimal digit of 1/92, skipping the first decimal zero)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t rem = 1;                    /* remainder of 1/92 after integer part */
    for (idx_t i = 0; i <= n; ++i)
    {
        rem *= (seq_t)10;
        seq_t digit = rem / (seq_t)92;
        rem       %= (seq_t)92;
        if (i == n)
            return digit;
    }
    return 0; /* unreachable */
}
