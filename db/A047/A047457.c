/*
 * A047457  Numbers that are congruent to {3, 4} mod 8.
 * https://oeis.org/A047457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-5 - 3*(-1)^n + 8*n)/2
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Closed form: a(n) = 4*n - 1 if n is odd; 4*(n-1) if n is even. */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    if ((n & 1) != 0) {
        return sn * (seq_t)4 - (seq_t)1;
    } else {
        return (sn - (seq_t)1) * (seq_t)4;
    }
}
