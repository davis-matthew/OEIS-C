/*
 * A180347  The number of n-digit numbers requiring 4 nonzero squares in their
 *          representation as sum of squares.
 * https://oeis.org/A180347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = # of n‑digit numbers of the form 4^k·(8b+7)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 10^(n‑1) and 10^n – 1 as unsigned __int128 for safe intermediate
       arithmetic – they fit into at most 136 bits for n ≤ 10000. */
    unsigned __int128 L = 1;          /* 10^(n-1) */
    for (idx_t i = 1; i < n; ++i)
        L *= 10U;
    unsigned __int128 U = L * 10U - 1; /* 10^n – 1 */

    seq_t result = 0;
    unsigned __int128 pow4 = 1;           /* 4^k */

    while (pow4 <= U) {
        unsigned __int128 t_min = (L + pow4 - 1) / pow4; /* ceil(L/4^k) */
        unsigned __int128 t_max = U / pow4;             /* floor(U/4^k) */

        if (t_max >= (unsigned __int128)7) {
            if (t_min <= t_max) {
                unsigned __int128 rem   = t_min % 8U;
                unsigned __int128 first = t_min + ((unsigned __int128)7 - rem + 8U) % 8U;
                if (first <= t_max) {
                    unsigned __int128 cnt = (t_max - first) / 8U + 1U;
                    result += (seq_t)cnt;
                }
            }
        }
        /* Stop to avoid overflow of pow4 on next multiplication. */
        if (pow4 > U / 4U)
            break;
        pow4 *= 4U;
    }

    return result;
}
