/*
 * A010719  Period 2: repeat {5,8}.
 * https://oeis.org/A010719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 3*(n mod 2) + 5 Equivalent recurrence: a(0)=5, a(1)=8,
 *            a(n)=a(n-2) for n>1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (n & 1) ? (seq_t)8 : (seq_t)5;
}
