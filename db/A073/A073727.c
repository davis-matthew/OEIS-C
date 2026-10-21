/*
 * A073727  Number of distinct-digit integers between squares.
 * https://oeis.org/A073727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of integers with all distinct decimal digits
 *            strictly between n² and (n+1)²
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Local helper: true if x has no repeated decimal digit */
static bool has_distinct_digits(uint64_t x)
{
    unsigned mask = 0;
    if (x == 0) return true;         /* single digit 0 is distinct */
    while (x) {
        unsigned d = (unsigned)(x % 10);
        unsigned bit = 1u << d;
        if (mask & bit) return false;
        mask |= bit;
        x /= 10;
    }
    return true;
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n must be >= 1 per OEIS offset.  No special handling for n<1. */
    uint64_t low  = (uint64_t)n * n + 1;          /* first integer > n²   */
    uint64_t high = (uint64_t)(n + 1) * (n + 1);  /* first integer >= (n+1)² */

    seq_t count = 0;
    for (uint64_t v = low; v < high; ++v) {
        if (has_distinct_digits(v))
            ++count;
    }
    return count;
}
