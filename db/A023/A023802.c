/*
 * A023802  Xenodromes: all digits in base 7 are different.
 * https://oeis.org/A023802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   a(n) = the n‑th nonnegative integer whose digits in base 7 are
 *            all * distinct (xenodromes)
 * Range:     u8 n <= 171, u16 n <= 5193, u32 n <= 9659, u64 n <= 9659,
 *            u128 n <= 9659, big any n
 * Helpers:   unique_base7_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/unique_base7_digits.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  We enumerate all non‑negative integers and return the
     * n‑th that has unique base‑7 digits. */
    idx_t count = 0;
    seq_t val   = 0;

    for (;;) {
        if (unique_base7_digits(val)) {
            ++count;
            if (count == n)
                return val;
        }
        val = val + (seq_t)1;
    }
}
