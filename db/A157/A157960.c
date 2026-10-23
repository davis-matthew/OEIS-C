/*
 * A157960  a(n) = 121*n^2 - n.
 * https://oeis.org/A157960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 121*n^2 - n
 * Range:     u8 n <= 1, u16 n <= 23, u32 n <= 5957, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the appropriate output type first */
    seq_t t = (seq_t)n;
    /* Compute 121 * n^2 - n efficiently without intermediate overflow
       (the arithmetic is performed in seq_t so it works for arbitrary‑precision). */
    t = t * ((seq_t)121 * t - (seq_t)1);
    return t;
}
