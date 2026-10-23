/*
 * A016958  a(n) = (6*n + 4)^2.
 * https://oeis.org/A016958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 4)^2
 * Range:     u8 n <= 1, u16 n <= 41, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type to avoid mixed‑type arithmetic */
    seq_t tn = (seq_t)n;
    /* Compute (6*n + 4) and square it */
    seq_t val = 6 * tn + 4;
    return val * val;
}
