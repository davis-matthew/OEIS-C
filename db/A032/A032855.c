/*
 * A032855  Numbers whose base-8 representation Sum_{i=0..m} d(i)*8^i has d(m)
 *          <= d(m-1) >= d(m-2) <= ...
 * https://oeis.org/A032855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑8 digits alternate in
 *            comparison starting with d_m ≤ d_{m−1} ≥ d_{m−2} ≤ … . The
 *            function enumerates positive integers, checks the property digit
 *            by digit, and returns the nth one
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_valid(seq_t val)
{
    if (val < (seq_t)8)          /* single‑digit numbers always satisfy */
        return true;

    /* buffer for at most 128 base‑8 digits (128 bits => < 43 digits) */
    uint8_t dig[128];
    int len = 0;
    seq_t x = val;

    while (x > (seq_t)0)          /* extract digits least significant first */
    {
        dig[len++] = (uint8_t)(x % 8);
        x = x / 8;
    }

    /* now dig[0] is least significant, dig[len‑1] most significant */
    bool less = true;             /* first comparison is <= */
    uint8_t prev = dig[len - 1];  /* most significant digit */

    for (int idx = len - 2; idx >= 0; --idx)
    {
        uint8_t cur = dig[idx];
        if (less)
        {
            if (!(prev <= cur))
                return false;
        }
        else
        {
            if (!(prev >= cur))
                return false;
        }
        less = !less;            /* toggle expectation */
        prev = cur;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Safety: n is unsigned, typically >=1 */
    idx_t count = 0;
    seq_t val = 1;               /* start searching from 1 */

    for (;;)
    {
        if (is_valid(val))
        {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
}
