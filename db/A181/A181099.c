/*
 * A181099  Exchange rightmost two ternary digits of n > 1; a(0)=0, a(1)=3.
 * https://oeis.org/A181099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 9*(n/9) + 3*(n%3) + ((n/3)%3) with initial values a(0)=0,
 *            a(1)=3
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 3;
    seq_t tn = (seq_t)n;          /* cast index to seq_t for arithmetic */
    seq_t part1 = (tn / 9) * 9;   /* 9 * floor(n/9) */
    seq_t part2 = 3 * (tn % 3);   /* 3 * (n mod 3) */
    seq_t part3 = (tn / 3) % 3;   /* (floor(n/3) mod 3) */
    return part1 + part2 + part3;
}
