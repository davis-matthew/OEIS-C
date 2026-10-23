/*
 * A044486  Numbers n such that string 0,3 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   contains_03
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A044486: 
   a(n) = the n-th integer whose base‑6 representation contains the
   substring “03” whereas the base‑6 representation of the successor
   does not.  The function computes terms by brute‑force scanning. */
#include <stdbool.h>
#include "../utils/contains_03.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t val = 1;
    while (1) {
        if (contains_03(val) && !contains_03(val + 1)) {
            count++;
            if (count == n) {
                return val;
            }
        }
        val++;
    }
}
