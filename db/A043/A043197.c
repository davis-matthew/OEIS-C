/*
 * A043197  Numbers k such that 2 and 5 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043197:
   Numbers k such that the base‑9 representation contains two
   adjacent digits 2 and 5 (in either order), but the base‑9
   representation of k‑1 does not contain such a pair.
   The function returns a(n) for n ≥ 1. */

#include <stdbool.h>

/* Return true iff x in base‑9 contains an adjacent pair 2 5 or 5 2. */
static bool has_adjacent_25or52(seq_t x)
{
    seq_t prev = 9;          /* sentinel larger than any base‑9 digit */
    while (x > 0)
    {
        seq_t cur = x % 9;
        if ((prev == 5 && cur == 2) || (prev == 2 && cur == 5))
            return true;
        prev = cur;
        x /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n < 1, but safe fallback */

    idx_t found = 0;          /* how many qualifying numbers have been seen */
    seq_t candidate = 1;      /* start scanning from 1 upwards */

    while (found < n)
    {
        /* candidate has an adjacent 2‑5 pair and candidate‑1 does not */
        if (has_adjacent_25or52(candidate) &&
            !has_adjacent_25or52(candidate - 1))
        {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }

    return 0;  /* unreachable, but keeps the function defined for all paths */
}
