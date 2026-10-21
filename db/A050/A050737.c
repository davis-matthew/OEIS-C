/*
 * A050737  Numbers of form 7^k (values of k see A050728) containing no pair
 *          of consecutive equal digits (probably finite).
 * https://oeis.org/A050737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 7^k where k is the nth exponent such that 7^k has no
 *            consecutive * equal decimal digits
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   no_consecutive_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/no_consecutive_digits.h"

/* Return the nth term of OEIS sequence A050737.  The
 * index n follows the OEIS offset: a(0) is the first
 * term listed. */
seq_t a(idx_t n)
{
    idx_t  count = 0;
    seq_t  val   = 1;            /* 7^0 */

    while (1) {
        if (no_consecutive_digits(val)) {
            if (count == n)
                return val;
            ++count;
        }
        val *= 7;                /* next power of 7 */
    }
}
