/*
 * A021302  Decimal expansion of 1/298.
 * https://oeis.org/A021302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/298) mod 10
 * Range:     u8 n <= 9, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n-th digit after the decimal point of 1/298.  The
   calculation uses only integer arithmetic and works for all
   supported sizes of seq_t and idx_t. */
seq_t a(idx_t n)
{
    seq_t r = 1;              /* r = 10^k mod 298, start with k=0 */
    for (idx_t i = 0; i < n; ++i)
    {
        r = (r * 10) % 298;   /* update r: r_{k+1} = (10*r_k) mod 298 */
    }
    return ((r * 10) / 298);  /* digit = floor(10*r / 298) */
}
