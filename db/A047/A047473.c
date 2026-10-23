/*
 * A047473  Numbers that are congruent to {2, 3} mod 8.
 * https://oeis.org/A047473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - (7 + 3*(-1)^n)/2 (offset 1, a(1)=2, a(2)=3)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; 0 is out of bounds – return 0 as a safe choice */
        return 0;
    seq_t mul4 = (seq_t)n * (seq_t)4;
    return (n & (idx_t)1) ? (mul4 - (seq_t)2) : (mul4 - (seq_t)5);
}
