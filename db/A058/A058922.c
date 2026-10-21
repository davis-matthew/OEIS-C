/*
 * A058922  a(n) = n*2^n - 2^n = 2^n*(n-1).
 * https://oeis.org/A058922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (n-1) (offset 1, a(1) = 0)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                 /* value below offset – not used, but safe */
        return (seq_t)0;

    /* compute 2^n (no overflow checks; overflow simply truncates as
     * required for fixed‑width builds). */
    seq_t power = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        power *= (seq_t)2;

    return power * ((seq_t)(n - (idx_t)1));
}
