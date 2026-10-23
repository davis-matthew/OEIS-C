/*
 * A047386  Numbers that are congruent to {0, 2, 5} mod 7.
 * https://oeis.org/A047386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/3) + 0 if n mod 3 == 1, * 2 if n mod 3 ==
 *            2, * 5 if n mod 3 == 0
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* nth term of numbers congruent to 0, 2, or 5 modulo 7. */
    idx_t q = (n - 1) / 3;          /* integer division gives the block number */
    seq_t base = (seq_t)q * (seq_t)7;
    idx_t r = n % 3;
    seq_t offset = (r == 1) ? (seq_t)0
                : (r == 2) ? (seq_t)2
                          : (seq_t)5;
    return base + offset;
}
