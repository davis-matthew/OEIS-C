/*
 * A115264  Diagonal sums of correlation triangle for floor((n+2)/2).
 * https://oeis.org/A115264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} Sum_{j=0..min(k, n-2k)} *
 *            floor((k-j+2)/2) * floor((n-2k-j+2)/2)
 * Range:     u8 n <= 17, u16 n <= 87, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute diagonal sums of correlation triangle for floor((n+2)/2) */
seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t half = n >> 1;                /* floor(n/2) */

    for (idx_t k = 0; k <= half; ++k) {
        /* j ranges from 0 up to min(k, n-2k) */
        idx_t maxj = k;
        if (n >= 2 * k) {
            idx_t alt = n - 2 * k;
            if (alt < maxj) {
                maxj = alt;
            }
        } else {
            /* For k > n/2 this cannot happen, but keep the logic safe */
            maxj = 0;
        }

        for (idx_t j = 0; j <= maxj; ++j) {
            /* floor((k-j+2)/2) and floor((n-2k-j+2)/2) as integer >>1 */
            idx_t term1 = (k - j + 2) >> 1;
            idx_t term2 = (n - 2 * k - j + 2) >> 1;
            total += (seq_t)term1 * (seq_t)term2;
        }
    }

    return total;
}
