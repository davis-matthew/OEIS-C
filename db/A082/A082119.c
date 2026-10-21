/*
 * A082119  Smallest positive difference between d and n/d for any divisor d
 *          of n.
 * https://oeis.org/A082119
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min_{d|n, d < n/d} (n/d - d) (positive differences only)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset is 2, behaviour for n<2 is irrelevant */
    seq_t best = 0;                /* 0 means “no difference found yet” */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            if (e > d) {
                seq_t diff = (seq_t)e - (seq_t)d;
                if (best == 0 || diff < best)
                    best = diff;
            }
        }
    }
    return best;
}
