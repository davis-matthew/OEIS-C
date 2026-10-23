/*
 * A145051  Numerator of the first convergent to sqrt(n) using the recursion x
 *          = (n/x + x)/2.
 * https://oeis.org/A145051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = (n + 1)/2 if n is odd, otherwise a(n) = n + 1 (1‑indexed)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    if (N & 1)            /* n is odd */
        return (N + 1) / 2;
    else                  /* n is even */
        return N + 1;
}
