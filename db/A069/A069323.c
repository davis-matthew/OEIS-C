/*
 * A069323  Triangle in which n-th row gives ascending sequence of numbers
 *          derived from the (3x+1) problem, beginning with n. Numbers in one
 *          row share the same number of iteration steps required to reach the
 *          value of '1' when applying the (3x+1) algorithm. Each row
 *          terminates with a power of 2.
 * https://oeis.org/A069323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_power_of_two, trailing_two_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A069323 – triangle of numbers with equal Collatz step counts.
 *
 * For a given starting value `first` (row header) produce the row
 *
 *     cur_0 = first
 *     cur_{k+1} = 6*cur_k + 2^{v2(cur_k)+1}
 *
 * where v2(x) is the exponent of the largest power of two dividing x.
 * The row ends when `cur_k` is a power of two; the final term of the row
 * is that power of two.  The flattened sequence is the concatenation
 * of all rows in increasing order of `first`.  This function returns
 * a(n) for a 1‑based index n.
 */

#include <stdbool.h>
#include "../utils/is_power_of_two.h"
#include "../utils/trailing_two_bits.h"

seq_t a(idx_t n)
{
    idx_t pos = 0;            /* current position in the flattened sequence */

    for (idx_t first = 1; ; ++first) {          /* iterate over rows */
        seq_t cur = (seq_t)first;               /* first term of the row */

        while (1) {
            ++pos;                              /* we are about to output cur */

            if (pos == n)
                return cur;                     /* desired term found */

            if (is_power_of_two(cur))
                break;                          /* row terminates */

            unsigned e = trailing_two_bits(cur);
            cur = 6 * cur + ((seq_t)1 << (e + 1));
        }
    }
}
