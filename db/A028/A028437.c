/*
 * A028437  Golc sequence in base 7. Left to right concatenation of
 *          n,int(log_7(n)),int(log_7(int(log_7(n)))),... in base7.
 * https://oeis.org/A028437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation in base 7 of n, ⌊log7(n)⌋,
 *            ⌊log7(⌊log7(n)⌋)⌋, …; the last 0 is included only when the
 *            preceding value is > 1 (except that a(1) = 7)
 * Range:     u8 n <= 36, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Append the base‑7 representation of 'v' to the accumulating
   value '*res'.  The digits are processed most significant to least
   significant.
   v is an idx_t (uint64_t).  For v==0, the single digit 0 is appended. */
static void append_base7(idx_t v, seq_t *res)
{
    if (v == 0)
    {
        *res = (*res) * 7 + 0;
        return;
    }

    unsigned char digs[64];
    int len = 0;
    while (v > 0)
    {
        digs[len++] = v % 7;
        v /= 7;
    }

    for (int i = len - 1; i >= 0; --i)
        *res = (*res) * 7 + digs[i];
}

/* floor(log_7(x)) for unsigned 64‑bit integers */
static inline idx_t floor_log7(idx_t x)
{
    idx_t l = 0;
    while (x >= 7)
    {
        x /= 7;
        ++l;
    }
    return l;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)            /* out of range guard, not used by partners */
        return 0;

    /* Special case n == 1 : 1 followed by 0 gives 7 */
    if (n == 1)
        return 7;

    seq_t res = 0;
    append_base7(n, &res);

    idx_t cur = n;
    while (true)
    {
        idx_t nxt = floor_log7(cur);
        if (nxt == 0)
        {
            /* Append the trailing 0 only if the preceding value
               is greater than 1 (so that a(7) stops and a(49)
               keeps the zero). */
            if (cur != 1)
                append_base7(0, &res);
            break;
        }

        append_base7(nxt, &res);
        cur = nxt;
    }
    return res;
}
