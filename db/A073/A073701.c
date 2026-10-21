/*
 * A073701  a(n) = n^2*a(n-1) + (-1)^n.
 * https://oeis.org/A073701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * a(n-1) + (-1)^n, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prev = (seq_t)1;          /* a(0) */
    if (n == 0)
        return prev;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* prev holds a(i‑1) */
        seq_t prod = (seq_t)i * (seq_t)i * prev;
        if (i & 1U)                /* odd i : (-1)^i = -1 */
            prod -= (seq_t)1;      /* subtraction is safe because prod ≥ 1 */
        else                        /* even i : (-1)^i = +1 */
            prod += (seq_t)1;
        prev = prod;               /* now prev == a(i) */
    }

    return prev;                    /* a(n) */
}
