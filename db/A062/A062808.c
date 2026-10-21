/*
 * A062808  a(n) = Sum_{i=1..n} n^i * (n - i).
 * https://oeis.org/A062808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} n^i * (n - i)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (This implementation works for n >= 1, with a(1) = 0.) */

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n <= 1)
        return 0;

    seq_t result = 0;
    seq_t power   = (seq_t)n;          /* power = n^1 initially */

    /* Sum terms for i = 1 .. n-1  (the term for i = n vanishes, n - n = 0) */
    for (idx_t i = 1; i < n; ++i)
    {
        seq_t term = power * (seq_t)(n - i);   /* n^i * (n - i)   */
        result += term;

        /* Advance to next power: power = n^{i+1} */
        power *= (seq_t)n;
    }

    return result;
}
