/*
 * A021802  Decimal expansion of 1/798.
 * https://oeis.org/A021802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = ⌊10^(n+1)/798⌋ mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th decimal digit of 1/798 by iterative remainder */
seq_t a(idx_t n)
{
    idx_t rem = 1;                /* current remainder (0 ≤ rem < 798) */
    for (idx_t i = 0; i <= n; ++i)
    {
        rem *= 10;                 /* bring next decimal place into view */
        seq_t digit = (seq_t)(rem / 798);   /* next digit */
        rem %= 798;                /* new remainder for next iteration */
        if (i == n)
            return digit;          /* final digit requested */
    }
    return 0; /* unreachable, keeps compiler happy */
}
