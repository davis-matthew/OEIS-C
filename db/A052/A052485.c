/*
 * A052485  Weak numbers (i.e., not powerful (1)): there is a prime p where
 *          p|n is true but p^2|n is not true.
 * https://oeis.org/A052485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 230, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9355 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the nth weak number by enumerating integers and testing
   for being powerful.  The nth weak number is the n-th integer >1
   for which at least one prime factor occurs to the first power. */

#include <stdint.h>
#include <stdbool.h>

/* Return true iff x is powerful (every prime divisor to exponent ≥2,
   with 1 considered powerful). */
static bool is_powerful(uint64_t x)
{
    if (x == 1)
        return true;
    for (uint64_t d = 2; d * d <= x; ++d)
    {
        if (x % d == 0)
        {
            int exp = 0;
            while (x % d == 0)
            {
                x /= d;
                ++exp;
            }
            if (exp == 1)
                return false;      /* an exponent‑1 factor ⇒ not powerful */
        }
    }
    /* If a prime factor > sqrt(x) remains, its exponent is 1. */
    return (x == 1);
}

/* Return the nth weak number (offset 1). */
seq_t a(idx_t n)
{
    uint64_t count = 0;
    for (uint64_t cand = 2; ; ++cand)        /* candidates start at 2 */
    {
        if (!is_powerful(cand))
        {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
    }
}
