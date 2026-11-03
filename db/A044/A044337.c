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
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: linear search for the nth positive integer whose decimal
   representation contains the substring "05" and whose predecessor does not. */

#include <stdbool.h>

/* Return true iff decimal representation of x contains the consecutive digits 0 followed by 5. */
static bool contains_05(seq_t x)
{
    int prev = -1;                 /* Previous digit (higher significance) */
    while (x > 0) {
        int digit = (int)(x % (seq_t)10);
        if (prev == 5 && digit == 0)
            return true;
        prev = digit;
        x = x / (seq_t)10;
    }
    return false;
}

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
