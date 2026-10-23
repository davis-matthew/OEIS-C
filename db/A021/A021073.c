/*
 * A021073  Decimal expansion of 1/69.
 * https://oeis.org/A021073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 69) % 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th decimal digit of 1/69 (a(0)=0, a(1)=1, …). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* the leading 0 before the decimal point */

    seq_t digit = 0;
    uint64_t rem = 1;      /* remainder before computing a digit */

    /* Perform one division for each digit up to a(n).  */
    for (idx_t i = 0; i <= n; ++i)
    {
        digit = (seq_t)((rem * 10) / 69);   /* current decimal digit */
        rem   = (rem * 10) % 69;             /* update remainder */
    }
    return digit;
}
