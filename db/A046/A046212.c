/*
 * A046212  First numerator and then denominator of central elements of
 *          Leibniz's Harmonic Triangle.
 * https://oeis.org/A046212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for all odd n; for even n (n = 2k), a(n) = (k/2) *
 *            binom(2k, k)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 31, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1) return 1;

    idx_t k = n >> 1;            /* n = 2k */
    seq_t res = 1;

    /* Compute binomial(2k, k) multiplicatively */
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(k + i)) / (seq_t)i;
    }

    /* Multiply by k/2, knowing the product is integral */
    res = res * (seq_t)k / 2;

    return res;
}
