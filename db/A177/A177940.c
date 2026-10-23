/*
 * A177940  Decimal expansion of 190/89.
 * https://oeis.org/A177940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = ((190 × 10^(n-1)) div 89) mod 10
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A177940: decimal digits of 190/89 (1‑indexed, a(1)=2) */

seq_t a(idx_t n)
{
    if (n == 0)         /* offset is 1, but guard against 0 */
        return 0;
    if (n == 1)
        return (seq_t)2;    /* integer part */

    idx_t rem = 190 % 89;    /* initial remainder after the integer part */
    for (idx_t i = 2; i <= n; ++i) {
        rem = rem * 10;      /* bring down next digit */
        seq_t digit = (seq_t)(rem / 89);   /* current decimal digit */
        rem = rem % 89;      /* new remainder for next step */
        if (i == n)
            return digit;
    }
    /* should never reach here */
    return 0;
}
