/*
 * A181303  Numbers of the form i*7^j-1 (i=1..6, j >= 0).
 * https://oeis.org/A181303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth smallest number of the form i·7^j − 1 (i = 1..6,
 *            j ≥ 0)
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t curpow[6];
    seq_t val[6];
    /* initialise the 6 geometric sequences (j = 0) */
    for (int i = 0; i < 6; ++i) {
        curpow[i] = (seq_t)1;
        val[i]    = (seq_t)(i + 1) * curpow[i] - (seq_t)1; /* i*7^0 - 1 = i */
    }

    idx_t count = 0;

    while (1) {
        /* find sequence with minimal current value */
        int min_idx = 0;
        for (int i = 1; i < 6; ++i)
            if (val[i] < val[min_idx])
                min_idx = i;

        seq_t current = val[min_idx];
        ++count;
        if (count == n)
            return current;

        /* advance the chosen sequence: j += 1 */
        curpow[min_idx] = curpow[min_idx] * ((seq_t)7);
        val[min_idx]    = (seq_t)(min_idx + 1) * curpow[min_idx] - (seq_t)1;
    }
}
