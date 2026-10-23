/*
 * A152054  Bouncy numbers (numbers whose digits are in neither increasing nor
 *          decreasing order).
 * https://oeis.org/A152054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th bouncy number (an integer whose digits are neither
 * strictly increasing nor strictly decreasing, i.e., the digits are neither
 * monotonic nondecreasing nor monotonic nonincreasing).
 * The sequence starts at 101, which is the first 3‑digit bouncy number.
 */

#include <stdbool.h>

/* Local helper: test whether a number is bouncy. */
static bool is_bouncy(seq_t x)
{
    /* Numbers with fewer than 2 digits cannot be bouncy. */
    if (x < 10) return false;

    seq_t prev = x % 10;
    x /= 10;

    bool inc = true;     /* digits never decreased so far */
    bool dec = true;     /* digits never increased so far */

    while (x > 0) {
        seq_t d = x % 10;
        if (d < prev) inc = false;
        if (d > prev) dec = false;
        prev = d;
        x /= 10;
    }

    return !inc && !dec;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t cur = 101;       /* the first bouncy number */
    idx_t count = 0;

    while (1) {
        if (is_bouncy(cur)) {
            ++count;
            if (count == n) return cur;
        }
        ++cur;
    }
}
