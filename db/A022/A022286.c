/*
 * A022286  a(n) = n*(29*n - 1)/2.
 * https://oeis.org/A022286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(29*n - 1)/2
 * Range:     u8 n <= 4, u16 n <= 67, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A022286.
 * It works for any integer type seq_t (unsigned, possibly multi‑precision) and
 * idx_t (the index).  All arithmetic is performed in seq_t to avoid
 * intermediate overflow of idx_t. */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = nn * ((seq_t)29 * nn - 1) / 2;
    return res;
}
