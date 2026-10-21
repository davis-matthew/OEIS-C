/*
 * A062289  Numbers n such that n-th row in Pascal triangle contains an even
 *          number, i.e., A048967(n) > 0.
 * https://oeis.org/A062289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer ≥2 that is NOT of the form 2^k‑1 (i.e.
 *            its binary representation contains the substring “10”)
 * Range:     u8 n <= 247, u16 n <= 8297, u32 n <= 8297, u64 n <= 8297,
 *            u128 n <= 8297, big any n
 * Helpers:   has_all_ones
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_all_ones.h"

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0; /* undefined for n<1 */

    seq_t count = (seq_t)0;
    seq_t num   = (seq_t)1;           /* start from 2 */

    /* search until the n‑th qualifying number is found */
    while (1) {
        ++num;
        if (has_all_ones(num)) /* skip numbers 2^k‑1 (including 1) */
            continue;
        ++count;
        if (count == n)
            return num;
    }
}
