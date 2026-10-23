/*
 * A174041  Composites of the form 6n+2 or 6n+3.
 * https://oeis.org/A174041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n-1)=6n+2, a(2n)=6n+3
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* n>=1 by definition; below offset is irrelevant */
    idx_t k = (n + 1) / 2;          /* ceil(n/2) */
    seq_t base = (seq_t)6 * (seq_t)k;
    return base + ((n & 1) ? (seq_t)2 : (seq_t)3);
}
