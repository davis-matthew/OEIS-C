/*
 * A239878  Numbers k with digit_sum(k*k) + 1 = digit_sum((k+1)*(k+1)).
 * https://oeis.org/A239878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k where digit_sum(k*k)+1 = digit_sum((k+1)*(k+1))
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 3127, u64 n <= 4067,
 *            u128 n <= 4067, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* offset 1: a(1) = 0 */
    if (n == 0)
        return 0;

    seq_t k     = 0;          /* current k */
    seq_t k_sq  = 0;          /* k*k (as seq_t) */
    idx_t  count = 0;         /* how many terms found so far */

    while (count < n) {
        /* (k+1)^2 = k^2 + 2*k + 1 */
        seq_t next_sq = k_sq + ((seq_t)2 * k) + 1;

        if (digit_sum(k_sq) + (seq_t)1 == digit_sum(next_sq)) {
            /* this k qualifies */
            count++;
            if (count == n)
                return k;
        }

        /* advance to k+1 */
        k++;
        k_sq = next_sq;
    }
    /* Should never reach here, but return last k to satisfy compiler */
    return k;
}
