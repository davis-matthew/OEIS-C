/*
 * A047303  Numbers that are congruent to {0, 1, 2, 3, 4, 6} mod 7.
 * https://oeis.org/A047303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) + floor(n/6)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A047303: numbers congruent to {0,1,2,3,4,6} mod 7 with offset 1. */
seq_t a(idx_t n)
{
    /* Since n ≥ 1, (n-1) and n/6 are well-defined integer values. */
    return (seq_t)(n - 1) + (seq_t)(n / 6);
}
