/*
 * A074066  Zigzag modulo 3.
 * https://oeis.org/A074066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1)=1; for n>1: a(3n−1)=3n+1, a(3n)=3n, a(3n+1)=3n−1.
 *            Equivalently (for n≥2): if n%3==0 → a(n)=n; if n%3==1 →
 *            a(n)=n−2; if n%3==2 → a(n)=n+2
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1u) {
        return (seq_t)1;
    }

    uint64_t r = n % 3u;

    if (r == 0u) {
        return (seq_t)n;
    } else if (r == 1u) {
        return (seq_t)n - (seq_t)2;
    } else { /* r == 2 */
        return (seq_t)n + (seq_t)2;
    }
}
