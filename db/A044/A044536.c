/*
 * A044536  Numbers n such that string 2,3 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044536
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

/* OEIS A044536: numbers n such that the base‑7 representation of n contains
   the substring “23” but the base‑7 representation of n+1 does not. */
#include <stdbool.h>

/* Return true iff x in base 7 contains the consecutive digits 2,3. */
static bool has_substring_23(seq_t x)
{
    if (x == 0)
        return false;
    int prev = -1;
    while (x != 0) {
        int curr = (int)(x % 7);
        if (prev == 3 && curr == 2)
            return true;
        prev = curr;
        x = x / 7;
    }
    return false;
}

/* Main sequence function.  a(1) is the first such number. */
seq_t a(idx_t n)
{
    if (n == 0)            /* the sequence is 1‑based; return 0 for n=0 */
        return 0;

    idx_t count = 0;
    seq_t cand = 1;

    for (;;) {
        if (has_substring_23(cand) && !has_substring_23(cand + 1)) {
            ++count;
            if (count == n)
                return cand;
        }
        ++cand;
    }
}
