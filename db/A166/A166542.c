/*
 * A166542  a(n) = 6*n - a(n-1), with n>1, a(1)=2.
 * https://oeis.org/A166542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 6; with a(1) = 2, a(2) = 10
 * Range:     u8 n <= 83, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent definition: a(n) = 6*floor(n/2) + (2 if n odd else 4) */
seq_t a(idx_t n)
{
    /* floor(n/2) computed in seq_t to keep all arithmetic in seq_t */
    seq_t half = ((seq_t)n) / 2;
    seq_t base = 6 * half;

    /* Add the constant offset depending on parity of n */
    if (n & 1)
        return base + 2;
    else
        return base + 4;
}
