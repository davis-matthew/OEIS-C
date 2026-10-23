/*
 * A100137  a(n) = Sum_{k=0..floor(n/6)} C(n-3k,3k) * 2^(n-6k).
 * https://oeis.org/A100137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/6)} C(n-3k,3k) * 2^(n-6k)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t) */

static const seq_t base[6] = {1,2,4,8,16,32};

seq_t a(idx_t n)
{
    if (n < 6) return base[n];

    seq_t result = 0;

    for (idx_t k = 0; k <= n/6; ++k)
    {
        idx_t tot = n - 3*k;          /* tot >= 3k */
        idx_t r   = 3*k;              /* choose r */

        /* compute C(tot, r) = ∏_{i=1..r} (tot-i+1)/i   (exact division) */
        seq_t comb = 1;
        for (idx_t i = 1; i <= r; ++i)
        {
            comb = comb * (seq_t)(tot - (i-1));
            comb = comb / (seq_t)i;   /* exact division */
        }

        seq_t term = comb * pow2(n - 6*k);
        result += term;
    }

    return result;
}
