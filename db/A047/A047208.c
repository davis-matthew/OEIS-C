/*
 * A047208  Numbers that are congruent to {0, 4} mod 5.
 * https://oeis.org/A047208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*(n-1) + (n-1 mod 2 ? 3 : 0)) / 2
 * Range:     u8 n <= 103, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047208: numbers that are 0 or 4 (mod 5).  */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* even n: a(n) = (5*n - 2)/2
       odd  n: a(n) = 5*(n-1)/2  */
    if ((nn & 1) == 0) {
        return ((seq_t)5 * nn - 2) / 2;
    } else {
        return ((seq_t)5 * (nn - 1)) / 2;
    }
}
