/*
 * A044337  Numbers n such that string 0,5 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   contains_05
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: linear search for the nth positive integer whose decimal
   representation contains the substring "05" and whose predecessor does not. */

#include <stdbool.h>
#include "../utils/contains_05.h"

/* OEIS sequence A044337 */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = 1;

    while (1) {
        if (contains_05(candidate) && !contains_05(candidate - (seq_t)1)) {
            count++;
            if (count == n)
                return candidate;
        }
        candidate++;
    }
}
