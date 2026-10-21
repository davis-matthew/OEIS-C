/*
 * A087142  Numbers divisible by their individual digits, but not by the sum
 *          of their digits (counted with multiplicity).
 * https://oeis.org/A087142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 697, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n‑th positive integer that
   - is divisible by each of its non‑zero decimal digits,
   - is NOT divisible by the sum of its digits.                                           */
#include <stdint.h>
#include <stdbool.h>

/* Test whether a 64‑bit integer satisfies the condition. */
static bool is_valid(uint64_t x)
{
    uint64_t sum = 0;
    uint64_t tmp = x;

    while (tmp > 0) {
        uint8_t d = (uint8_t)(tmp % 10);
        if (d == 0) return false;          /* any zero digit invalidates the number */
        if (x % d != 0) return false;      /* not divisible by this digit */
        sum += d;
        tmp /= 10;
    }
    /* sum > 0 here; if divisible by sum, reject */
    return (x % sum) != 0;
}

/* Return the n‑th term of A087142 (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined index sentinel */

    uint64_t candidate = 1;
    idx_t    found = 0;

    while (found < n) {
        if (is_valid(candidate)) {
            found++;
            if (found == n)
                return (seq_t)candidate;
        }
        candidate++;
    }
    /* control never reaches here */
    return 0;
}
