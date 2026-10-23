/*
 * A008642  Quarter-squares repeated.
 * https://oeis.org/A008642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+4)/4) * floor((n+6)/4)
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS A008642 sequence (Quarter‐squares repeated). */
seq_t a(idx_t n)
{
    /* Integer division in C truncates toward zero, the same as floor for positives. */
    seq_t left  = ((seq_t)(n + 4)) / 4;
    seq_t right = ((seq_t)(n + 6)) / 4;
    return left * right;
}
