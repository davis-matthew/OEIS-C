/*
 * A032521  Sum of the integer part of 14/3-th roots of integers less than n.
 * https://oeis.org/A032521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{n} ⌊k^(3/14)⌋
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 68 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sum is taken over all integers k ≤ n.  The first few terms
 *  match the OEIS sequence A032521.
 */

#include <stdint.h>

/* Compute m^(14) using 128‑bit arithmetic to avoid overflow.
   m is small (≤ 10 for n up to 10 000), but the routine is generic. */
static unsigned __int128 pow14(uint64_t m)
{
    unsigned __int128 r = 1;
    /* 14 multiplications, safe because m^14 fits in 128 bits for m ≤ 10^4 */
    for (int i = 0; i < 14; ++i)
        r *= m;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    uint64_t current_m = 0;          /* floor(k^(3/14)) for current k */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute k^3 safely in 128 bits */
        unsigned __int128 cube = (unsigned __int128)k * k * k;

        /* increment current_m as long as (current_m+1)^14 ≤ k^3 */
        while (pow14(current_m + 1) <= cube)
            ++current_m;

        /* add floor value to the cumulative sum */
        sum += (seq_t)current_m;
    }

    return sum;
}
