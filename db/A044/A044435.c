/*
 * A044435  Numbers k such that string 0,2 occurs in the base-3 representation
 *          of k but not of k+1.
 * https://oeis.org/A044435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   has02
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that the base‑3 representation of k contains
   the substring "02" while the base‑3 representation of k+1 does not.
   Compute by sequentially testing each k until the nth one is found. */

#include <stdbool.h>
#include "../utils/has02.h"

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;          /* undefined for n < 1 */

    seq_t zero = (seq_t)0;
    seq_t one  = (seq_t)1;
    idx_t  count = 0;
    seq_t k = zero;

    while (true) {
        if (has02(k)) {
            seq_t kp1 = k + one;
            if (!has02(kp1)) {
                ++count;
                if (count == n)
                    return k;
            }
        }
        k = k + one;
    }
}
