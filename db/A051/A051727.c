/*
 * A051727  Denominator of n(n-1)(n-2)/720.
 * https://oeis.org/A051727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 720 / gcd(n*(n-1)*(n-2), 720)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominator of n(n-1)(n-2)/720 */
seq_t a(idx_t n)
{
    /* For n = 0,1,2 the numerator is zero, conventionally the denominator is 1 */
    if (n < 3u)
        return 1;

    /* Count the exponents of 2, 3, and 5 in the product n*(n-1)*(n-2) */
    idx_t v2 = 0, v3 = 0, v5 = 0;
    idx_t terms[3] = { n, n - 1, n - 2 };

    for (int i = 0; i < 3; ++i) {
        idx_t t = terms[i];

        /* factor 2 */
        while ((t & 1u) == 0) {
            ++v2;
            t >>= 1;
        }

        /* factor 3 */
        while (t % 3u == 0) {
            ++v3;
            t /= 3u;
        }

        /* factor 5 */
        while (t % 5u == 0) {
            ++v5;
            t /= 5u;
        }
    }

    /* 720 = 2^4 * 3^2 * 5^1 */
    idx_t exp2 = v2 < 4u ? v2 : 4u;
    idx_t exp3 = v3 < 2u ? v3 : 2u;
    idx_t exp5 = v5 < 1u ? v5 : 1u;

    /* Compute the gcd as 2^exp2 * 3^exp3 * 5^exp5 */
    idx_t g = 1;
    for (idx_t i = 0; i < exp2; ++i) g *= 2u;
    for (idx_t i = 0; i < exp3; ++i) g *= 3u;
    for (idx_t i = 0; i < exp5; ++i) g *= 5u;

    /* Return the denominator */
    return (seq_t)(720u / g);
}
