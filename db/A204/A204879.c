/*
 * A204879  Numbers that can be written as sum of perfect numbers.
 * https://oeis.org/A204879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth number in the set { 6k : k>0 } ∪ { 2k : k>25 } ∪
 *            {28,34,40,46} The algorithm enumerates the even numbers in
 *            increasing order and selects those that belong to the set
 * Range:     u8 n <= 114, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t val    = 6;          /* start from the smallest even perfect sum */

    for (;; val += 2) {        /* iterate over all even numbers */
        bool in_set;

        if (val >= (seq_t)52) {
            /* all even numbers greater than 50 are included */
            in_set = true;
        } else {
            /* for numbers <= 50 only multiples of 6 and the special cases 28,34,40,46 */
            if (val % (seq_t)6 == 0) {
                in_set = true;
            } else if (val == (seq_t)28 || val == (seq_t)34 ||
                       val == (seq_t)40 || val == (seq_t)46) {
                in_set = true;
            } else {
                in_set = false;
            }
        }

        if (in_set) {
            count++;
            if (count == n)
                return val;
        }
    }
}
