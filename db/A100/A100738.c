/*
 * A100738  Inverse modulo 2 binomial transform of 5^n.
 * https://oeis.org/A100738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{k , bit k of n = 1} (5^{2^k} - 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    static seq_t factor[64] = {0};
    static bool ready[64]  = {false};
    seq_t result = (seq_t)1;
    idx_t m = n;

    while (m != 0)
    {
        /* get index of lowest set bit */
        idx_t k   = __builtin_ctzll(m);
        idx_t val = (idx_t)1 << k;

        if (!ready[k])
        {
            factor[k] = pow_seq((seq_t)5, val);   /* 5^{2^k} */
            ready[k]  = true;
        }

        result *= (factor[k] - (seq_t)1);  /* multiply by (5^{2^k} - 1) */
        m &= m - 1;                        /* clear lowest set bit */
    }
    return result;
}
