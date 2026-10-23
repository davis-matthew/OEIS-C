/*
 * A044177  Numbers k such that string 5,4 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1500, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_54_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_54_base7.h"

/* Finds the n-th integer k such that the base‑7 representation of k contains
   the substring 54 but the base‑7 representation of k‑1 does not.  The
   function is efficient enough for n ≈ 10000 (≈ O(k) with k ≈ a(n)).  */

seq_t a(idx_t n)
{
    seq_t count = 0;   /* how many suitable k have been found */
    seq_t k = 1;       /* candidate integer to test */

    while (1) {
        if (has_54_base7(k) && !has_54_base7(k - 1)) {
            count++;
            if (count == (seq_t)n)   /* nth found */
                return k;
        }
        k++; /* next candidate */
    }
}
