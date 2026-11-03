/*
 * A033058  Numbers whose base-7 representation Sum_{i=0..m} d(i)*7^i has odd
 *          d(m-i) for all odd i.
 * https://oeis.org/A033058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: Generate the nth positive integer whose base‑7 digits satisfy
 *   : For every odd i, the digit at position (m-i) is odd
 *   : equivalently, in the L‑digit base‑7 representation (L = m+1),
 *     digits at positions j with parity different from L-1 are odd.
 * The function returns the nth term (a(1)=1) for all n ≥ 1.
 */

#include <stdbool.h>

static bool valid_base7(seq_t x)
{
    /* Reject 0 although never used in a() */
    if (x == 0) return false;

    /* Extract base‑7 digits into an array (least‑significant first). */
    seq_t tmp   = x;
    int   digs[64];
    int   L = 0;

    while (tmp != 0)
    {
        digs[L++] = (int)(tmp % 7);
        tmp      /= 7;
    }

    /* Check the parity condition. */
    for (int j = 0; j < L; ++j)
    {
        /* digit is required odd iff (j ^ (L-1)) is odd. */
        if ( ((j ^ (L - 1)) & 1) != 0 )
        {
            if ( (digs[j] & 1) == 0 ) return false;  /* even digit fails */
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset is 1, not used */

    seq_t  candidate = 1;
    idx_t  count     = 0;

    while (1)
    {
        if (valid_base7(candidate))
        {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
