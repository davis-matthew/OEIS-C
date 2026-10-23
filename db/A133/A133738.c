/*
 * A133738  Expansion of product of 3rd order mock theta function phi(q) and
 *          Ramanujan theta function f(-q) in powers of q.
 * https://oeis.org/A133738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 328, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A133738 – expansion of φ(q)·f(−q)
 *  Formula (OEIS notation):
 *  a(0) = 1,
 *  a(n) = 2 * Σ_{k≥1} Σ_{m≥0} (−1)^{k+m}
 *         [ 1_{n = k(3k+1)/2 + 2k m} + 1_{n = k(3k+3)/2 + 2k m} ]   (n>0)
 *
 *  Each term of the bivariate sum contributes +1 or –1 to a(n).
 *  The factor 2 accounts for the symmetry k ↔ –k in the original
 *  sum over all integers k.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;

    for (idx_t k = 1; ; ++k)
    {
        /* S1 = k*(3k+1)/2,  S2 = k*(3k+3)/2  (both fit in seq_t) */
        seq_t S1 = (seq_t)k * ((seq_t)3 * k + 1) / 2;
        if (S1 > (seq_t)n)
            break;

        idx_t diff = n - (idx_t)S1;
        if (diff % (2 * k) == 0)
        {
            idx_t m = diff / (2 * k);
            int sgn = ((k + m) & 1) ? -1 : 1;
            sum += (seq_t)sgn;
        }

        seq_t S2 = (seq_t)k * ((seq_t)3 * k + 3) / 2;
        if (S2 <= (seq_t)n)
        {
            diff = n - (idx_t)S2;
            if (diff % (2 * k) == 0)
            {
                idx_t m = diff / (2 * k);
                int sgn = ((k + m) & 1) ? -1 : 1;
                sum += (seq_t)sgn;
            }
        }
    }

    return sum * 2;
}
