/*
 * A039258  Numbers whose base-12 representation has the same number of 5's
 *          and 10's.
 * https://oeis.org/A039258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th non‑negative integer whose base‑12 representation
   contains an equal number of digits 5 and 10. */
#include <stdbool.h>

static inline bool equal_five_ten(seq_t x)
{
    seq_t tmp = x;
    idx_t c5 = 0;
    idx_t c10 = 0;
    const seq_t base = (seq_t)12;
    const seq_t five = (seq_t)5;
    const seq_t ten  = (seq_t)10;
    while (tmp > 0)
    {
        seq_t d = tmp % base;
        if (d == five)
            c5++;
        else if (d == ten)
            c10++;
        tmp /= base;
    }
    return c5 == c10;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t val = 0;
    while (found < n)
    {
        if (equal_five_ten(val))
        {
            found++;
            if (found == n)
                return val;
        }
        val++;
    }
    /* (unreachable) */
    return (seq_t)0;
}
