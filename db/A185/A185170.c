/*
 * A185170  a(n) = floor( (2*n^2 - 6*n + 9) / 5).
 * https://oeis.org/A185170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2*n^2 - 6*n + 9) / 5)
 * Range:     u8 n <= 12, u16 n <= 192, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert to seq_t early to avoid unintended narrowing. */
    seq_t nn = (seq_t)n;
    seq_t val = (2 * nn * nn + 9) - 6 * nn;  /* 2*n^2 - 6*n + 9, always ≥ 0 for n ≥ 0 */
    return val / 5;                          /* integer division = floor for non‑negative values */
}
