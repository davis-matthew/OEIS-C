/*
 * A067408  Eighth column of triangle A067402.
 * https://oeis.org/A067408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 15 * 64^(n-1) for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is A067408 – an easy geometric progression. */

seq_t a(idx_t n)
{
    /* Handle the offset explicitly. */
    if (n == 0)
        return (seq_t)1;

    /* Start with the value for n = 1: 15 * 64^0 = 15. */
    seq_t value = (seq_t)15;

    /* Multiply by 64 (2^6) for each additional term. */
    for (idx_t i = 1; i < n; ++i)
        value *= (seq_t)64;

    return value;
}
