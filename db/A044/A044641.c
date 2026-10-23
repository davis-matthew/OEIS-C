/*
 * A044641  Numbers k such that string 1,0 occurs in the base 9 representation
 *          of k but not of k+1.
 * https://oeis.org/A044641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 81 * b(n) + 9 where b(n) is the n‑th non‑negative integer
 *            that contains no substring "10" in its base‑9 representation
 *            (with 0 itself allowed)
 * Range:     u8 n <= 4, u16 n <= 711, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_no10_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_no10_base9.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t q     = 0;
    while (1) {
        if (has_no10_base9(q)) {
            count++;
            if (count == n)
                return ((seq_t)81 * (seq_t)q) + (seq_t)9;
        }
        q++;
    }
}
