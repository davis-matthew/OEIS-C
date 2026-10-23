/*
 * A182622  a(n) is the number whose binary representation is the
 *          concatenation of the divisors of n written in base 2.
 * https://oeis.org/A182622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the integer whose binary representation is the
 *            concatenation of the binary representations of the divisors of n
 *            in ascending order
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 47, u64 n <= 143, u128 n <= 719,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;      /* safety for n=0, not used */

    /* arrays to hold divisors; maximum number of divisors for n≤10000 is <200 */
    idx_t small[256];
    idx_t large[256];
    idx_t sc = 0, lc = 0, i;

    /* find divisors */
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            small[sc++] = i;
            if (i != n / i) {
                large[lc++] = n / i;
            }
        }
    }

    seq_t result = 0;

    /* process small divisors in ascending order */
    for (i = 0; i < sc; ++i) {
        idx_t d = small[i];
        /* number of bits needed for d */
        idx_t bits = 0;
        idx_t tmp = d;
        while (tmp) {
            tmp >>= 1;
            ++bits;
        }
        result = (result << bits) | (seq_t)d;
    }

    /* process large divisors in ascending order (reverse of 'large' array) */
    for (i = lc; i > 0; --i) {
        idx_t d = large[i - 1];
        idx_t bits = 0;
        idx_t tmp = d;
        while (tmp) {
            tmp >>= 1;
            ++bits;
        }
        result = (result << bits) | (seq_t)d;
    }

    return result;
}
