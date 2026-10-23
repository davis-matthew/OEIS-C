/*
 * A045538  Smallest number k such that kn contains two consecutive digits
 *          which are the same.
 * https://oeis.org/A045538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = minimal positive k such that the decimal representation
 *            of n*k contains two consecutive equal digits. The implementation
 *            performs a simple linear search from k = 1 upwards
 * Range:     u8 n <= 20, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   no_consecutive_equal_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/no_consecutive_equal_digits.h"

seq_t a(idx_t n)
{
    if (n == 0) { return 0; }          /* 0 is not defined in the sequence */
    seq_t nseq = (seq_t)n;

    for (seq_t k = 1;; ++k) {
        seq_t prod = nseq * k;
        if (!no_consecutive_equal_digits(prod))
            return k;
    }
}
