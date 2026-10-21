/*
 * A074995  Floor of concatenation of n, n+1, n+2, n+3, n+4 divided by 5.
 * https://oeis.org/A074995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor( concat(n,n+1,n+2,n+3,n+4) / 5 ) where concat
 *            concatenates the decimal representations of the five consecutive
 *            integers n, n+1, ..., n+4. This function works with the
 *            arbitrary‑precision seq_t type, but is also efficient for 64‑bit
 *            builds
 * Range:     u8 none, u16 n <= 5, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precompute powers of 10 up to 10^20 (enough for an idx_t). */
    static seq_t pow10[21];
    static bool init = false;
    if (!init) {
        pow10[0] = 1;
        for (int i = 1; i < 21; ++i) {
            pow10[i] = pow10[i - 1] * 10;
        }
        init = true;
    }

    seq_t concat = 0;

    for (idx_t offset = 0; offset < 5; ++offset) {
        idx_t num_idx = n + offset;   /* still fits in idx_t (uint64_t) */
        seq_t num = (seq_t)num_idx;

        /* Count decimal digits of num_idx, with 0 having one digit. */
        int digits;
        if (num_idx == 0) {
            digits = 1;
        } else {
            digits = 0;
            idx_t t = num_idx;
            while (t != 0) {
                t /= 10;
                ++digits;
            }
        }

        /* concat = concat * 10^digits + num */
        concat = concat * pow10[digits] + num;
    }

    return concat / 5;   /* floor division */
}
