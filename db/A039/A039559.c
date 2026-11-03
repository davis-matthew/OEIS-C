/*
 * A039559  Numbers whose base-4 representation has the same number of 0's,
 *          1's and 2's.
 * https://oeis.org/A039559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for numbers whose base‑4 representation
   contains the same number of digits 0, 1, and 2.  The function scans
   integers in increasing order, testing the digit counts until the
   requested term is found. */
#include <stdint.h>
#include <stdbool.h>

/* Helper: check whether the base‑4 digits of v have equal counts of 0, 1
   and 2.  Leading zeros are not part of the representation. */
static bool equal_0_1_2(seq_t v)
{
    int c0 = 0, c1 = 0, c2 = 0;
    seq_t x = v;
    while (x) {
        int d = (int)(x % 4);      /* low base‑4 digit */
        if (d == 0) {
            ++c0;
        } else if (d == 1) {
            ++c1;
        } else if (d == 2) {
            ++c2;
        }
        x /= 4;
    }
    return c0 == c1 && c1 == c2;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t val = 1;        /* start testing from 1 */
    idx_t found = 0;      /* how many valid numbers seen */

    while (found < n) {
        if (equal_0_1_2(val)) {
            ++found;
            if (found == n) {
                return val;
            }
        }
        val = val + 1;
    }
    return val;           /* unreachable but keeps compiler happy */
}
