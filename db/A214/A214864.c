/*
 * A214864  Numbers k such that k XOR 10 = k - 10.
 * https://oeis.org/A214864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*floor((n-1)/4) + [10,11,14,15][(n-1) mod 4]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1, undefined for n<1 */
    idx_t m = n - 1;                      /* m = (n-1) */
    seq_t offsets[4] = { (seq_t)10, (seq_t)11, (seq_t)14, (seq_t)15 };
    seq_t base = (seq_t)16 * (seq_t)(m / 4);  /* 16 * floor((n-1)/4) */
    return base + offsets[m % 4];
}
