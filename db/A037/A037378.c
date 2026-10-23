/*
 * A037378  Positive numbers k such that every base-2 digit of k is a base-9
 *          digit of k.
 * https://oeis.org/A037378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer k such that every base‑2 *
 *            digit of k is also a base‑9 digit of k. Equivalently, the base‑9
 *            * representation of k must contain the digit 1, and, unless k =
 *            2^m‑1 * (i.e. k has only 1‑bits), it must also contain the digit
 *            0. * (Numbers of the form 2^m‑1 are allowed...
 * Range:     u8 n <= 26, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t k = 1;
    while (found < n) {
        /* check whether k's base‑9 representation contains 1 and 0 */
        bool has1 = false;
        bool has0 = false;
        seq_t t = k;
        if (t == 0) {          /* no effect – k never 0 in this loop */
            has1 = false;
            has0 = true;
        } else {
            while (t > 0) {
                unsigned digit = (unsigned)(t % 9);
                if (digit == 1) has1 = true;
                if (digit == 0) has0 = true;
                t /= 9;
            }
        }

        if (has1) {
            /* if k is not of the form 2^m‑1, zero must appear as well */
            if (has0 || (((k + 1) & k) == 0)) {
                ++found;
                if (found == n)
                    return k;
            }
        }
        /* advance to next integer */
        k = k + 1;
    }
    /* unreachable: the loop will always return */
    return 0;
}
