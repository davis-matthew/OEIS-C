/*
 * A065571  Numbers whose decimal digits can be permuted to give a multiple of
 *          11.
 * https://oeis.org/A065571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   has_11_permutation
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Check whether the decimal digits of a number can be permuted to form a multiple of 11 */
/* a(0) = 0, a(n) is the nth smallest non‑negative integer with that property. */
/* The algorithm tests each integer in turn (starting at 0) and uses a DP over
   digit subsets to decide if a permutation divisible by 11 exists. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_11_permutation.h"

/* OEIS sequence A065571 */
seq_t a(idx_t n)
{
    idx_t found = 0;
    uint64_t num = 0;
    while (found <= n) {
        if (has_11_permutation(num)) {
            if (found == n)
                return (seq_t)num;
            ++found;
        }
        ++num;
    }
    /* should never reach here */
    return (seq_t)0;
}
