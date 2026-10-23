/*
 * A103173  Numbers k such that the decimal digits of k are not present in
 *          k^2, k^3, or k^4.
 * https://oeis.org/A103173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   digit_mask, good_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: search over k to find the n‑th integer k such that none of the decimal digits of k occur in k^2, k^3, or k^4. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_mask.h"
#include "../utils/good_k.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t cnt = 0;
    seq_t k = 1;
    while (cnt < n)
    {
        if (good_k(k))
        {
            ++cnt;
            if (cnt == n) return k;
        }
        ++k;
    }
    return 0; /* unreachable */
}
