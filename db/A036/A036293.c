/*
 * A036293  a(n) = n*7^n.
 * https://oeis.org/A036293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*7^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 7^n iteratively.  This approach guarantees correctness
     * for every seq_t type that supports signed/unsigned integer
     * arithmetic, whether it is 8/16/32/64/128 bits or an arbitrary‑precision
     * integer type.  The multiplication is performed using the native
     * arithmetic of seq_t, so overflow naturally occurs when the result
     * does not fit, which is acceptable because the sequence is only
     * checked for values that fit into seq_t.  For 64‑bit seq_t this
     * loop runs at most 21 times for values that fit, and for larger n
     * the result is irrelevant.  For arbitrary‑precision builds the
     * loop correctly computes 7^n for any n. */
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i)
        pow *= (seq_t)7;

    return pow * (seq_t)n;
}
