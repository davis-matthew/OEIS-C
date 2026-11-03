/*
 * A031479  Numbers whose base-6 representation has 2 more 0's than 5's.
 * https://oeis.org/A031479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑6 representation has 2 more 0's than 5's.
   The sequence is obtained by brute‑force enumeration of natural
   numbers in increasing order and testing the digit‑count condition. */
/* The function conforms to the required signature: seq_t a(idx_t n). */
#include <stdbool.h>

/* Return true if x in base 6 has two more 0 digits than 5 digits. */
static inline bool has_two_more_zeros_than_fives(seq_t x)
{
    int zero = 0;
    int five = 0;
    while (x > 0) {
        int d = (int)(x % 6);   /* d ∈ {0,…,5} */
        if (d == 0)
            zero++;
        else if (d == 5)
            five++;
        x /= 6;
    }
    return zero == five + 2;
}

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
