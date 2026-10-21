/*
 * A058224  Largest d such that the linear programming bound for quantum codes
 *          of length n is feasible for some real K>1.
 * https://oeis.org/A058224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)/6) + floor((n+2)/6) + 1
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; the formula works for n ≥ 1 */
    seq_t ns = (seq_t)n;
    return (ns + 1) / 6 + (ns + 2) / 6 + 1;
}
