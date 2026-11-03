/*
 * A052192  Smallest a(n) for which n*a(n) has no isolated digits.
 * https://oeis.org/A052192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nice, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 56, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9652 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A052192 by brute‑force search:
   For each n, find the smallest a such that n*a has no isolated digits
   (in the decimal representation) that is, every digit  has a
   neighboring digit with the same value. */

#include <stdint.h>
#include <stdbool.h>

/* Static helper: test whether all digits of m are non‑isolated. */
static bool no_isolated(seq_t m)
{
    /* Maximum number of decimal digits for uint128 is 39, use buffer of 200. */
    uint8_t digits[200];
    unsigned int len = 0;

    /* Special case: if m is 0, treat as isolated. */
    if (m == 0)
        return false;

    /* Extract digits least‑significant first. */
    while (m != 0) {
        digits[len++] = (uint8_t)(m % 10);
        m /= 10;
    }

    if (len == 1)   /* single digit can never have a matching neighbour */
        return false;

    /* Check each digit has at least one equal neighbour. */
    for (unsigned int i = 0; i < len; ++i) {
        uint8_t d = digits[i];
        bool left  = (i > 0     && digits[i-1] == d);
        bool right = (i + 1 < len && digits[i+1] == d);
        if (!left && !right)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t prod = 0;
    for (seq_t a_val = 1; ; ++a_val) {
        prod += (seq_t)n;                /* prod = n * a_val   */
        if (no_isolated(prod))
            return a_val;                /* desired minimal a */
    }
}
