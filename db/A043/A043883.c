/*
 * A043883  Numbers k such that 1 and 2 occur juxtaposed in the base-4
 *          representation of k but not of k+1.
 * https://oeis.org/A043883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043883: Numbers k such that the base‑4 representation of k
   contains adjacent digits 1 and 2 (in either order) but the base‑4
   representation of k+1 does not.  Computed by brute‑force search. */

#include <stdbool.h>
#include <stdint.h>

static bool has_adj_12or21(idx_t x)
{
    /*
     * Return true if the base‑4 representation of x contains the
     * pattern "12" or "21".  We build the digits least significant first
     * and then scan for adjacency.
     */
    unsigned char digits[64];
    int len = 0;

    do
    {
        digits[len++] = (unsigned char)(x & 3); /* x % 4 */
        x >>= 2;                                /* x /= 4 */
    } while (x);

    for (int i = 1; i < len; ++i)
    {
        unsigned char a = digits[i];
        unsigned char b = digits[i - 1];
        if ((a == 1 && b == 2) || (a == 2 && b == 1))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0;; ++k)
    {
        if (has_adj_12or21(k) && !has_adj_12or21(k + 1))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
