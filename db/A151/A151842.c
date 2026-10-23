/*
 * A151842  a(3n) = n, a(3n+1) = 2n+1, a(3n+2) = n+1.
 * https://oeis.org/A151842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3*n) = n * a(3*n+1) = 2*n + 1 * a(3*n+2) = n + 1 * (OEIS
 *            A151842, offset 0)
 * Range:     u8 n <= 384, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* division and modulus by 3 are inexpensive for any uint64_t */
    idx_t mod = n % 3;
    seq_t m   = (seq_t)(n / 3);

    if (mod == 0)
        return m;          /* a(3*n)   = n */
    else if (mod == 1)
        return (seq_t)(2 * m + 1); /* a(3*n+1) = 2*n + 1 */
    else
        return (seq_t)(m + 1);     /* a(3*n+2) = n + 1 */
}
