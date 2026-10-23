/*
 * A031479  Numbers whose base-6 representation has 2 more 0's than 5's.
 * https://oeis.org/A031479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_two_more_zeros_than_fives
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑6 representation has 2 more 0's than 5's.
   The sequence is obtained by brute‑force enumeration of natural
   numbers in increasing order and testing the digit‑count condition. */
/* The function conforms to the required signature: seq_t a(idx_t n). */
#include <stdbool.h>
#include "../utils/has_two_more_zeros_than_fives.h"

/* a(n): n-th number (offset 1) whose base‑6 representation has
   two more 0's than 5's. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t x = 0;
    while (1) {
        if (has_two_more_zeros_than_fives(x)) {
            count++;
            if (count == n)
                return x;
        }
        x++;   /* next integer to test */
    }
}
