/*
 * A230456  Odd evil a(n) (A001969), such that 3*a(n) and a(n)+3 are odious
 *          (A000069).
 * https://oeis.org/A230456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th odd evil number such that 3*a(n) and a(n)+3 are
 *            odious. (computed by brute‑force search)
 * Range:     u8 n <= 4, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-01)
 * Helpers:   popcount_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/popcount_seq.h"

static inline bool _is_odd_evil(seq_t x)
{
    return ((x & 1) != 0) && ((popcount_seq(x) & 1) == 0);
}

static inline bool _is_odious(seq_t x)
{
    return ((popcount_seq(x) & 1) != 0);
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t candidate = 1;
    while (found < n)
    {
        if (_is_odd_evil(candidate))
        {
            seq_t three = candidate * 3;
            seq_t plus3 = candidate + 3;
            if (_is_odious(three) && _is_odious(plus3))
            {
                ++found;
                if (found == n)
                    return candidate;
            }
        }
        candidate += 2;   /* only odd numbers */
    }
    return 0;   /* unreachable for valid n>=1 */
}
