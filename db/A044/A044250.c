/*
 * A044250  Numbers n such that string 7,7 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 994, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A044250.  For each n≥1 it returns the nth
   integer whose base‑8 representation contains the substring "77"
   while the (n‑1)’s base‑8 representation does not.  The function
   performs a simple linear search; it is O(n) for the nth term and
   works for any seq_t type supported by the build. */
#include <stdbool.h>
#include <stdint.h>

/* Return true if x (written in base 8) contains two consecutive digits 7. */
static bool has77(seq_t x)
{
    uint8_t prev = 0;
    while (x > 0) {
        uint8_t cur = (uint8_t)(x & 7); /* digit = x % 8 */
        if (prev == 7 && cur == 7) return true;
        prev = cur;
        x >>= 3; /* x = x / 8 */
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1, but safe */

    seq_t candidate = 0;
    idx_t found = 0;

    while (found < n) {
        ++candidate; /* start check from 1 upwards */
        if (has77(candidate) && !has77(candidate - 1)) {
            ++found;
        }
    }
    return candidate;
}
