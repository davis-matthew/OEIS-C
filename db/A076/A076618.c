/*
 * A076618  Least x>1 such that x^d == 1 (mod d) for each divisor d of n.
 * https://oeis.org/A076618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of distinct prime factors of n + 1 (squarefree
 *            kernel of n plus 1)
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rad = 1;          /* accumulate the product of distinct primes */
    idx_t m = n;            /* work on a copy */

    /* handle factor 2 */
    if (m % 2 == 0) {
        rad = rad * (seq_t)2;
        while (m % 2 == 0) m /= 2;
    }

    /* odd factors, test up to sqrt(m) without overflow */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            rad = rad * (seq_t)p;
            while (m % p == 0) m /= p;
        }
    }

    /* last prime factor, if > 1 */
    if (m > 1) rad = rad * (seq_t)m;

    return rad + (seq_t)1;
}
