/*
 * A021141  Decimal expansion of 1/137.
 * https://oeis.org/A021141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th digit after the decimal point of 1/137
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The digit can be obtained by standard long division:
       r_0 = 1,
       for k = 0 … n-1: r_{k+1} = (r_k * 10) % 137
       a(n) = (r_n * 10) / 137
   The remainder r_k always satisfies 0 ≤ r_k < 137, so a(n) is a single
   decimal digit (0 – 9). */

seq_t a(idx_t n)
{
    seq_t r = 1;                 /* r_0 = 1 */
    for (idx_t i = 0; i < n; ++i)
        r = (r * 10) % 137;      /* advance one digit position */

    return (r * 10) / 137;       /* the digit at position n */
}
