/*
 * A032991  If d,e are consecutive digits of n in base 9, then |d-e|>=4.
 * https://oeis.org/A032991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A032991 – numbers n such that in base 9 every pair of
 *            consecutive digits differs by at least 4. Computed by scanning
 *            integers and testing the base‑9 digit property
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool is_ok(uint64_t x)
{
    if (x < 9) return true;        /* single-digit numbers are fine */
    int prev = x % 9;
    x /= 9;
    while (x > 0) {
        int cur = x % 9;
        int diff = cur - prev;
        if (diff < 0) diff = -diff;
        if (diff < 4) return false;
        prev = cur;
        x /= 9;
    }
    return true;
}

/* Returns the n-th element of OEIS A032991 (offset 1). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t val   = 0;
    while (count < n) {
        ++val;
        if (is_ok(val)) ++count;
    }
    return (seq_t)val;
}
