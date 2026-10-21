/*
 * A218621  a(n) = unique divisor d of n such that d + (n/d - 1)/2 is minimal
 *          and integral.
 * https://oeis.org/A218621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = unique divisor d of n for which the value d + (n/d - 1)/2
 *            is minimal and integral (i.e. n/d must be odd)
 * Range:     u8 n <= 255, u16 n <= 2048, u32 n <= 2048, u64 n <= 2048,
 *            u128 n <= 2048, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* out of range, not used in OEIS |

    /* Separate the largest power of two from n. */
    idx_t temp = n;
    idx_t k = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++k;
    }
    seq_t pow2 = ((seq_t)1) << k;   /* 2^k as seq_t */
    seq_t m    = (seq_t)temp;       /* odd part of n */

    seq_t best_d   = 0;    /* best divisor candidate (d = 2^k * d') */
    seq_t best_val = 0;    /* minimal value of d + (m/d - 1)/2 */
    int   first = 1;      /* flag to handle the first iteration */

    for (idx_t i = 1; ((seq_t)i * (seq_t)i) <= m; ++i) {
        if (m % (seq_t)i == 0) {
            seq_t dprime1 = (seq_t)i;
            seq_t cand1   = pow2 * dprime1;
            seq_t t1      = ((m / dprime1) - 1) / 2;
            seq_t f1      = cand1 + t1;
            if (first || f1 < best_val) {
                best_d   = cand1;
                best_val = f1;
                first = 0;
            }

            seq_t dprime2 = m / dprime1;
            if (dprime2 != dprime1) {
                seq_t cand2 = pow2 * dprime2;
                seq_t t2    = ((m / dprime2) - 1) / 2;   /* = (i - 1) / 2 */
                seq_t f2    = cand2 + t2;
                if (f2 < best_val) {
                    best_d   = cand2;
                    best_val = f2;
                }
            }
        }
    }
    return best_d;
}
