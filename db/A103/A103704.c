/*
 * A103704  Add 5 to each of the preceding digits, beginning with 1.
 * https://oeis.org/A103704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=1; for n≥2, a(n)=11 iff the second most significant bit of
 *            n is 1, otherwise a(n)=6
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    /* Find highest power of two ≤ n */
    idx_t high = 1;
    while (high <= n / 2) high <<= 1;
    /* Check if the bit just below the leading 1 is set */
    if (n & (high >> 1)) return (seq_t)11;
    return (seq_t)6;
}
