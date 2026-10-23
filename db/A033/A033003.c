/*
 * A033003  Every run of digits of n in base 5 has length 2.
 * https://oeis.org/A033003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 6*A043309(n) (numbers whose base‑5 expansion has runs of
 *            digits of length exactly 2)
 * Range:     u8 n <= 8, u16 n <= 84, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   map_except
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/map_except.h"

seq_t a(idx_t n)
{
    /* determine the number of digit pairs m */
    idx_t m   = 0;
    unsigned __int128 pow4 = 1;   /* 4^m */
    unsigned __int128 cum  = 0;   /* cumulative count up to current m */

    while (cum < n)
    {
        ++m;
        pow4 *= 4;   /* 4^m */
        cum  += pow4;
    }

    /* rank among the strings of length m (0‑based) */
    idx_t prev_total = (idx_t)(cum - pow4); /* terms with fewer than m pairs */
    idx_t rank       = n - prev_total - 1;

    /* extract the m base‑4 digits of rank (most significant first) */
    uint8_t digits4[64];               /* m will never exceed 64 for 64‑bit idx_t */
    for (int i = (int)m - 1; i >= 0; --i)
    {
        digits4[i] = rank % 4;
        rank /= 4;
    }

    /* convert to the actual run‑digit sequence in base‑5 */
    uint8_t actual[64];
    for (int i = 0; i < (int)m; ++i)
    {
        if (i == 0)
            actual[i] = 1 + digits4[i];       /* first pair cannot be 0 */
        else
            actual[i] = map_except(digits4[i], actual[i - 1]);
    }

    /* build the base‑5 number from the pair sequence */
    seq_t num = 0;
    for (int i = 0; i < (int)m; ++i)
    {
        num = num * 5 + actual[i];
        num = num * 5 + actual[i];
    }

    return num; /* a(n) itself */
}
