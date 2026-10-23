/*
 * A045185  Numbers whose base-5 representation contains exactly three 0's and
 *          one 2.
 * https://oeis.org/A045185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force enumeration of integers,
   counting those whose base‑5 representation contains exactly
   three 0's and one 2. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 1, but safe */

    idx_t cnt = 0;                 /* number of qualifying numbers found */
    uint64_t i = 250;              /* first qualifying number in base‑10 */

    for (;; ++i) {
        uint64_t t = i;
        int zeros = 0, twos = 0;

        while (t) {
            int d = t % 5;
            if (d == 0) ++zeros;
            else if (d == 2) ++twos;
            t /= 5;
        }

        if (zeros == 3 && twos == 1) {
            ++cnt;
            if (cnt == n)
                return (seq_t)i;   /* cast to the build's seq_t type */
        }
    }
}
