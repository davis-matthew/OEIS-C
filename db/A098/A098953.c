/*
 * A098953  Slowest increasing sequence where each number is such that at
 *          least one pair of adjacent digits are consecutive, the first of
 *          the pair being the smaller.
 * https://oeis.org/A098953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9583 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth integer ≥12 whose decimal representation
   contains at least one pair of adjacent digits where the second digit
   is exactly one larger than the first.  The index n is 1‑based
   (a(1) = 12).  The implementation scans the decimal digits from
   most significant to least significant to test the adjacency condition. */
#include <stdbool.h>

/* Test whether a number has an adjacent digit pair a,b with b = a + 1
   where a is the more significant digit.  The digits are examined from
   most significant to least significant. */
static bool has_consecutive_adjacent(seq_t x)
{
    /* At most 40 decimal digits for 128‑bit numbers; sufficient for any
       seq_t implementation needed here. */
    unsigned char digits[64];
    idx_t len = 0;

    if (x == 0)
        return false;          /* single‑digit numbers do not qualify */

    /* Extract digits in reverse order. */
    while (x > 0) {
        digits[len++] = (unsigned char)(x % 10);
        x /= 10;
    }

    /* Scan original order: digits[len-1] is most significant. */
    for (idx_t j = len - 1; j > 0; --j) {
        if (digits[j - 1] == digits[j] + 1)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)               /* safety for out‑of‑range indices */
        return (seq_t)0;

    seq_t val = (seq_t)12;     /* first term of the sequence */
    idx_t count = 1;

    if (n == 1)
        return val;

    while (count < n) {
        ++val;
        if (has_consecutive_adjacent(val))
            ++count;
    }
    return val;
}
