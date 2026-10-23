/*
 * A120891  Number of primitive Pythagorean triangles with odd leg 2n-1.
 * https://oeis.org/A120891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 1 a(n) = 2^(k-1) for n > 1, where k = number of
 *            distinct prime factors of 2n-1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)0;
    }

    /* m = 2n-1  (odd integer) */
    idx_t m = (idx_t)2 * n - (idx_t)1;
    idx_t k = 0;  /* count of distinct prime factors */

    /* Trial division by odd numbers up to sqrt(m) */
    for (idx_t d = 3; d <= m / d; d += 2) {
        if ((m % d) == 0) {
            ++k;
            while ((m % d) == 0) {
                m /= d;
            }
        }
    }
    if (m > 1) {
        /* m itself is a prime > sqrt(original m) */
        ++k;
    }

    /* a(n) = 2^(k-1) */
    return ((seq_t)1) << (k - 1);
}
