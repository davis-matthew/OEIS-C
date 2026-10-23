/*
 * A044227  Numbers n such that string 5,0 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer m such that oct(m) contains "50" and
 *            oct(m-1) does not
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   oct_has_50
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/oct_has_50.h"

/* Return the n-th member of sequence A044227 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0u)   /* offset starts at 1; n==0 is unused but safe */
        return 0;

    seq_t cur = 0;
    idx_t count = 0;

    while (1) {
        cur += 1;
        if (oct_has_50(cur) && !oct_has_50(cur - 1)) {
            ++count;
            if (count == n)
                return cur;
        }
    }
}
