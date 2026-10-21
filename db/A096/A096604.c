/*
 * A096604  Numbers that appear at most once in A096607.
 * https://oeis.org/A096604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer ≥3 that is not a power of two, and if it
 *            is odd then it must be of the form 2^k – 1 (the odd Mersenne
 *            numbers). Thus the sequence consists of all even numbers except
 *            powers of two, together with the odd numbers 3, 7, 15, 31, …
 *            (2^k‑1)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t num = 3;          /* sequence starts with 3 */

    for (;; ++num) {
        bool ok = false;

        if (!is_power_of_two(num)) {
            if (num & 1) {                /* odd number */
                if (is_power_of_two(num + 1))
                    ok = true;            /* only odd Mersenne numbers */
            } else {                     /* even number */
                ok = true;                /* all evens except powers of two */
            }
        }

        if (ok) {
            ++count;
            if (count == n)
                return num;
        }
    }
}
