/*
 * A061721  Number of zeros in the character table of the dihedral group with
 *          2n elements.
 * https://oeis.org/A061721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = (n-1)/2 if n is odd; * a(n) = n-2 if n ≡ 2 (mod 4); *
 *            a(n) = 2*(n/2-1) + # { (k,j) | 1≤k,j< n/2, * (k*j) mod n = n/4
 *            or 3n/4 } if n ≡ 0 (mod 4)
 * Range:     u8 n <= 111, u16 n <= 8639, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety – offset starts at 1 */

    /* odd n */
    if (n & 1ULL)
        return (seq_t)(n - 1ULL) / 2;

    /* n ≡ 2 (mod 4) */
    if ((n & 3ULL) == 2ULL)
        return (seq_t)(n - 2ULL);

    /* n ≡ 0 (mod 4) */
    idx_t m   = n / 2ULL;                 /* m = n/2 = 2*t */
    seq_t zeros = (seq_t)2 * (m - 1ULL);  /* zeros from the two reflection columns */
    idx_t t = n / 4ULL;                   /* n/4  (also 3n/4 is 3*t) */

    /* count zeros coming from the rotation classes */
    for (idx_t k = 1ULL; k < m; ++k) {
        for (idx_t j = 1ULL; j < m; ++j) {
            idx_t prod = k * j;
            idx_t rem  = prod % n;
            if (rem == t || rem == 3ULL * t)
                ++zeros;
        }
    }
    return zeros;
}
