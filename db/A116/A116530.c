/*
 * A116530  a(n) = 3*a(n-1), with a(1) = 20.
 * https://oeis.org/A116530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = 20 * 3^(n‑1)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 20, a(n) = 3 * a(n‑1) for n > 1 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n == 0 return 0
       to avoid an underflow in the loop counter. */
    if (n == 0)
        return 0;

    seq_t result = 20;
    for (idx_t i = 1; i < n; ++i)
        result *= (seq_t)3;
    return result;
}
