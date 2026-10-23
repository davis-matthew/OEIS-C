/*
 * A048711  2nd row of Family 1 "90 X 150 array": generations 0 .. n of Rule
 *          90 starting from seed pattern 7.
 * https://oeis.org/A048711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product('((bit_i((n+1), i)*(2^(2^(i+1))))+1)',
 *            'i'=0..floor_log_2(n+2)) * + 2*product('((bit_i(n,
 *            i)*(2^(2^(i+1))))+1)', 'i'=0..floor_log_2(n+1));
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    seq_t prod1 = (seq_t)1;
    seq_t prod2 = (seq_t)1;

    /* product for (n+1) */
    idx_t m1 = n + (idx_t)1;
    idx_t limit1 = floor_log2_uint64(n + (idx_t)2);
    for (idx_t i = 0; i <= limit1; ++i)
    {
        if ((m1 >> i) & (idx_t)1)
        {
            idx_t exp = ((idx_t)1 << (i + (idx_t)1));
            seq_t factor = pow2(exp) + (seq_t)1;
            prod1 *= factor;
        }
    }

    /* product for n */
    idx_t m2 = n;
    idx_t limit2 = floor_log2_uint64(n + (idx_t)1);
    for (idx_t i = 0; i <= limit2; ++i)
    {
        if ((m2 >> i) & (idx_t)1)
        {
            idx_t exp = ((idx_t)1 << (i + (idx_t)1));
            seq_t factor = pow2(exp) + (seq_t)1;
            prod2 *= factor;
        }
    }

    seq_t result = prod1 + (seq_t)2 * prod2;
    return result;
}
