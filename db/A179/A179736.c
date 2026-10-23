/*
 * A179736  Number of permutations of 1..n with the sequence of sums of 9
 *          adjacent elements having exactly 1 maximum.
 * https://oeis.org/A179736
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n-8) / 2^(n-9) for n ≥ 9, a(n) = 0 for n < 9
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 13, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 9) return (seq_t)0;

    /* exponent of 2 that must be divided out */
    idx_t e = n - 9;

    seq_t res = 1;

    for (idx_t i = 2; i <= n; ++i) {
        idx_t x = i;
        idx_t v = 0;
        /* count powers of 2 in i */
        while ((x & 1) == 0) {
            x >>= 1;
            ++v;
        }
        /* cancel as many of the needed 2's as possible */
        idx_t t = (v < e) ? v : e;
        if (t > 0) e -= t;

        /* multiply by i with t factors of 2 removed */
        res *= (seq_t)(i >> t);
    }

    /* after the loop e should be zero; multiply by (n-8) */
    res *= (seq_t)(n - 8);
    return res;
}
