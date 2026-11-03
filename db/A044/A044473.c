/*
 * A044473  Numbers n such that string 3,0 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044473: a(n) = nth integer k such that
 * the base‑5 representation of k contains the substring "30"
 * while the base‑5 representation of k+1 does not. */

#include <stdbool.h>

/* Returns true if x contains the digit pattern 3,0 in base 5. */
static bool has_30_base5(seq_t x)
{
    if (x == 0)
        return false;               /* single digit 0 cannot contain "30" */

    seq_t prev = x % 5;          /* least significant digit */
    x = x / 5;                   /* remaining higher digits */

    while (x > 0) {
        seq_t curr = x % 5;      /* next higher digit */
        if (curr == 3 && prev == 0)
            return true;         /* found "30" (3 then 0 in MSB→LSB order) */
        prev = curr;             /* slide window one digit to the left */
        x = x / 5;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t  count = 0;
    seq_t  x = 0;

    while (true) {
        if (has_30_base5(x) && !has_30_base5(x + 1)) {
            count++;
            if (count == n)
                return x;
        }
        x++;   /* move to next integer */
    }
}
