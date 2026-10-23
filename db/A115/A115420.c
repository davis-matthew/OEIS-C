/*
 * A115420  Numbers having a 1 in position 4 of their binary expansion.
 * https://oeis.org/A115420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n + 15 + 16*floor((n-1)/16)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115420: numbers having a 1 in position 4 of their binary expansion */

seq_t a(idx_t n) {
    seq_t x   = (seq_t)n;     /* convert index to seq_t */
    seq_t n1  = x - 1;        /* n-1 */
    seq_t q   = n1 / 16;      /* floor((n-1)/16) */
    seq_t r   = n1 % 16;      /* (n-1) mod 16 */
    return 32u * q + 16 + r; /* 32*q + 16 + r */
}
