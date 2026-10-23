/*
 * A021312  Decimal expansion of 1/308.
 * https://oeis.org/A021312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0)=0, a(1)=0; for n≥2, a(n)=pattern[(n-2) mod 6] where pattern
 *            = {3,2,4,6,7,5}. This implements the OEIS recurrence
 *            a(n)=a(n-1)-a(n-3)+a(n-4) for n>5, with the given initial values
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the first two terms explicitly. */
    if (n < 2)
        return 0;

    /* Repeat pattern of six digits for the rest of the sequence. */
    static const seq_t pat[6] = {3, 2, 4, 6, 7, 5};

    /* (n-2) is safe because we already handled n<2. */
    return pat[(n - 2) % 6];
}
