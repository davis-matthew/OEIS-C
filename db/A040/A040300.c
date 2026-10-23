/*
 * A040300  Continued fraction for sqrt(318).
 * https://oeis.org/A040300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=17; for n≥1: a(n)=1 if n is odd, a(n)=4 if n≡2 (mod 4),
 *            a(n)=34 if n≡0 (mod 4)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)17;
    }
    /* If n is odd */
    if ((n & 1ULL) == 1ULL) {
        return (seq_t)1;
    }
    /* n is even */
    if ((n & 3ULL) == 0ULL) { /* divisible by 4 */
        return (seq_t)34;
    }
    /* n ≡ 2 mod 4 */
    return (seq_t)4;
}
