/*
 * A208895  Number of non-congruent solutions to x^2 + y^2 + z^2 + t^2 == 1
 *          (mod n).
 * https://oeis.org/A208895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^3 * Π_{p|n, p odd} (1-1/p^2)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t res = (seq_t)n;
    res *= (seq_t)n;      /* n^2 */
    res *= (seq_t)n;      /* n^3 */

    /* Factor n, but ignore powers of 2 (they do not affect the product) */
    idx_t m = n;
    while ((m & 1) == 0)
        m >>= 1;          /* remove all factors of 2 */

    /* Trial division over odd primes */
    idx_t p = 3;
    while (p <= m / p) { /* avoid overflow on p*p */
        if (m % p == 0) {
            /* Multiply by (1 - 1/p^2) = (p^2-1)/p^2 */
            seq_t pp = (seq_t)p * (seq_t)p;      /* p^2 */
            res = (res / pp) * (pp - 1);         /* exact division */
            while (m % p == 0)
                m /= p;   /* strip all copies of this prime */
        }
        p += 2;
    }

    /* Remaining prime factor, if any */
    if (m > 1) {
        seq_t pp = (seq_t)m * (seq_t)m;
        res = (res / pp) * (pp - 1);
    }

    return res;
}
