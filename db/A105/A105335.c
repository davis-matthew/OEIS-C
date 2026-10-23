/*
 * A105335  a(n) = n*(n+1)/2 mod 64.
 * https://oeis.org/A105335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 mod 64
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the sequence is periodic with period 128, so we reduce n modulo 128
       to avoid overflow and keep the computation fast for any n */
    idx_t m = n % 128u;            /* m is in [0,127] */
    seq_t val = ((seq_t)m * ((seq_t)m + 1u)) / 2u; /* exact triangular number */
    return val & (seq_t)63u;       /* keep only the lower 6 bits (mod 64) */
}
