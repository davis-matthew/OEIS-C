/*
 * A181194  Number of n X 7 matrices containing a permutation of 1..n*7 in
 *          increasing order rowwise, columnwise, diagonally and (downwards)
 *          antidiagonally.
 * https://oeis.org/A181194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 720*a(n-1) - 84448*a(n-2) + 1503360*a(n-3) -
 *            17912224*a(n-4) - 318223104*a(n-5) + 564996096*a(n-6) +
 *            270471168*a(n-7) - 11373824*a(n-8) + 65536*a(n-9) (empirical
 *            recurrence, a(1)=1, a(2)=132, …, a(9)=1562998915785444034816)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* Small table of the first nine terms, parsed once from strings */
    static seq_t init[9];
    static bool init_done = false;
    if (!init_done) {
        init[0] = seq_from_str("1");
        init[1] = seq_from_str("132");
        init[2] = seq_from_str("49100");
        init[3] = seq_from_str("25097600");
        init[4] = seq_from_str("14083834704");
        init[5] = seq_from_str("8092149471168");
        init[6] = seq_from_str("4673805856338368");
        init[7] = seq_from_str("2702482348019033600");
        init[8] = seq_from_str("1562998915785444034816");
        init_done = true;
    }

    if (n <= 9)
        return init[n - 1];

    /* Recurrence requires the previous nine values */
    seq_t buf[9];
    /* buf[0] = a(k-1), buf[1] = a(k-2), …, buf[8] = a(k-9) */
    for (idx_t i = 0; i < 9; ++i)
        buf[i] = init[8 - i];

    for (idx_t k = 10; k <= n; ++k) {
        seq_t pos = ((seq_t)720) * buf[0]
                  + ((seq_t)1503360) * buf[2]
                  + ((seq_t)564996096) * buf[5]
                  + ((seq_t)270471168) * buf[6]
                  + ((seq_t)65536) * buf[8];

        seq_t neg = ((seq_t)84448) * buf[1]
                  + ((seq_t)17912224) * buf[3]
                  + ((seq_t)318223104) * buf[4]
                  + ((seq_t)11373824) * buf[7];

        seq_t val = pos - neg;

        /* shift buffer for next step */
        for (idx_t i = 8; i > 0; --i)
            buf[i] = buf[i - 1];
        buf[0] = val;
    }

    return buf[0];
}
