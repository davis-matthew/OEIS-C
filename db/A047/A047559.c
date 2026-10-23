/*
 * A047559  Numbers that are congruent to {0, 1, 3, 6, 7} mod 8.
 * https://oeis.org/A047559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 8*floor((n-1)/5) + offset[(n-1)%5] where offset =
 *            {0,1,3,6,7}
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t offsets[5] = { (seq_t)0, (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)7 };
    if (n < 1) return (seq_t)0; /* a(0) is undefined; return 0 for safety */
    idx_t k = (n - 1) / 5;
    idx_t r = (n - 1) % 5;
    seq_t val = (seq_t)k * (seq_t)8;
    return val + offsets[r];
}
