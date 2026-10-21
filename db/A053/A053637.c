/*
 * A053637  a(n) = ceiling(2^(n-1)/n).
 * https://oeis.org/A053637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(2^(n-1)/n)
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 38, u64 n <= 71, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  Handle a tiny safety case. */
    if (n == 0) return 0;

    /* Perform binary long division of 2^(n‑1) by n.
       The dividend has a single 1‑bit at position n-1.  We
       process its bits from most significant to least, keeping
       a 64‑bit remainder (which is always < n) and building the
       quotient bit by bit.  This avoids representing the huge
       power of two explicitly.  For 64‑bit builds the calculation
       will overflow only for n where the true result itself
       does not fit in uint64_t, which is acceptable. */
    seq_t q = 0;          /* quotient accumulated so far            */
    idx_t r = 0;          /* remainder, always < n                  */

    for (idx_t i = 0; i < n; ++i)
    {
        /* The most significant bit (i == 0) is 1, the rest are 0. */
        r = (r << 1) + (i == 0 ? 1 : 0);

        if (r >= n)
        {
            r -= n;
            q = (q << 1) | 1;
        }
        else
        {
            q = q << 1;
        }
    }

    /* If the division was not exact, we need the ceiling. */
    if (r) ++q;
    return q;
}
