/*
 * A238976  a(n) = ((3^(n-1) - 1)^2)/4.
 * https://oeis.org/A238976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((3^(n-1) - 1)^2)/4
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 1 the definition gives zero */
    if (n <= (idx_t)1)
        return (seq_t)0;

    /* Compute 3^(n‑1) */
    seq_t power = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        power *= (seq_t)3;

    /* Compute (3^(n-1) - 1)^2 / 4 */
    seq_t t = power - (seq_t)1;
    return (t * t) / (seq_t)4;
}
