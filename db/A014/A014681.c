/*
 * A014681  Fix 0; exchange even and odd numbers.
 * https://oeis.org/A014681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; for n≥1, a(n)=n+1 if n is odd, otherwise a(n)=n-1 (n≥2)
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n & (idx_t)1) {
        return (seq_t)n + (seq_t)1;   /* odd n */
    } else {
        return (seq_t)n - (seq_t)1;   /* even n ≥ 2 */
    }
}
