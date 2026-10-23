/*
 * A045144  Numbers whose base-4 representation contains exactly two 2's and
 *          one 3.
 * https://oeis.org/A045144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n-th smallest integer whose base‑4 representation
   contains exactly two 2's and one 3.  The implementation simply
   enumerates positive integers in increasing order and counts those
   that satisfy the digit condition. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* sequence starts at n = 1 */

    idx_t found = 0;
    unsigned long long cur = 1;

    for (;; ++cur) {
        unsigned long long tmp = cur;
        unsigned char ct2 = 0, ct3 = 0;

        while (tmp) {
            unsigned digit = tmp % 4;
            if (digit == 2) ++ct2;
            else if (digit == 3) ++ct3;
            tmp /= 4;
        }

        if (ct2 == 2 && ct3 == 1) {
            ++found;
            if (found == n)
                return (seq_t)cur;
        }
    }
}
