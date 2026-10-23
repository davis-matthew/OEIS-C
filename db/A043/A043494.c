/*
 * A043494  Numbers having two 1's in base 10.
 * https://oeis.org/A043494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 5720, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043494: Numbers having exactly two digit '1's in base 10. */
/* Computes the n-th such number in increasing order by brute‑force search. */

#include <stdbool.h>

/* Count whether an unsigned integer x has exactly two decimal digits equal to 1. */
static inline bool has_two_ones(idx_t x)
{
    int ones = 0;
    while (x) {
        if (x % 10 == 1) {
            ++ones;
            if (ones > 2)
                return false;
        }
        x /= 10;
    }
    return ones == 2;
}

/* Return the n-th integer with exactly two '1' digits in its decimal representation. */
seq_t a(idx_t n)
{
    idx_t i = 1;      /* current integer to test */
    idx_t count = 0;  /* how many valid numbers seen so far */

    while (1) {
        if (has_two_ones(i)) {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
        ++i;
    }
}
