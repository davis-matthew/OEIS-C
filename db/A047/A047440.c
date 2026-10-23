/*
 * A047440  Numbers that are congruent to {2, 5, 6} mod 8.
 * https://oeis.org/A047440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n - c)/3 with c = 2 if n % 3 == 1, c = 1 if n % 3 ==
 *            2, and c = 6 if n % 3 == 0. This yields the numbers congruent to
 *            2, 5, or 6 modulo 8 in increasing order
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not part of sequence; safeguard */
    seq_t prod = (seq_t)8 * (seq_t)n;
    seq_t c;
    switch (n % 3) {
        case 1: c = 2; break;
        case 2: c = 1; break;
        default: c = 6; break;      /* n % 3 == 0 */
    }
    return (prod - c) / 3;
}
