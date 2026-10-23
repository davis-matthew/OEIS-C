/*
 * A021957  Decimal expansion of 1/953.
 * https://oeis.org/A021957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/953) mod 10 (the n-th digit of 1/953 after
 *            the decimal point)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Long‑division routine to obtain the n‑th digit of the decimal expansion of 1/953. */
    idx_t rem   = 1u;      /* current remainder of the division */
    seq_t digit = 0;       /* digit to return */

    for (idx_t i = 0; i <= n; ++i)
    {
        digit = (seq_t)(rem * 10u) / 953u;   /* next digit      */
        rem   = (rem * 10u) % 953u;          /* update remainder */
    }
    return digit;
}
