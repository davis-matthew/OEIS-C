/*
 * A028390  Nearest integer to 3n/4 unless that is an integer, when 3n/2.
 * https://oeis.org/A028390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { if n mod 4 = 0 then 3*n/2 else round(3*n/4) }
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t three_n = (seq_t)n * (seq_t)3;
    if ((n % 4) == 0) {
        return three_n / (seq_t)2;   /* use 3n/2 for multiples of 4 */
    } else {
        return (three_n + (seq_t)2) / (seq_t)4; /* round(3n/4) */
    }
}
