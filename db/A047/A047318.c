/*
 * A047318  Numbers that are congruent to {0, 1, 2, 4, 5, 6} mod 7.
 * https://oeis.org/A047318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n-4)/6)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified integer formula: a(n) = n + ((n+2)/6) - 1 */

seq_t a(idx_t n)
{
    /* In the OEIS offset a(1) = 0. For n = 0 we return 0 as a fallback. */
    if (n == 0)
        return 0;

    /* (n + 2) / 6 is always non‑negative and fits in idx_t for typical use. */
    seq_t add = (seq_t)((n + 2) / 6);

    /* Compute a(n) using unsigned arithmetic; the result is valid for all n ≥ 1
       within the range of seq_t. */
    return (seq_t)n + add - 1;
}
