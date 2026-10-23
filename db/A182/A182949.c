/*
 * A182949  Joint-rank array of the numbers (3*i+1)*3^j, where i>=0, j>=0, by
 *          antidiagonals.
 * https://oeis.org/A182949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = rank of (3*i+1)*3^j where (i,j) is the pair enumerated by
 *            antidiagonals of the array indexed by i>=0, j>=0
 * Range:     u8 n <= 16, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* find diagonal s such that T_s < n <= T_{s+1}, where T_s = s*(s+1)/2 */
    idx_t s = 0;
    while ((s + 1u) * (s + 2u) / 2u < n)
        ++s;

    idx_t T_s = s * (s + 1u) / 2u;      /* number of entries before diagonal s */
    idx_t k   = n - T_s - 1u;           /* 0‑based index within diagonal s */

    idx_t i = k;            /* i increases from 0 along the diagonal */
    idx_t j = s - k;        /* j = s - i */

    /* value corresponding to (i,j) */
    seq_t v = (seq_t)(3u * i + 1u) * pow3(j);

    /* count all pairs (u,v) with (3*u+1)*3^v <= v – this is a(n) */
    seq_t count  = 0;
    seq_t power  = 1;      /* 3^v */

    while (true) {
        seq_t t    = v / power;                /* floor(v / 3^v) */
        seq_t imax = (t - 1u) / 3u;             /* max u for this v */
        count += imax + 1u;                    /* i = 0 .. imax */

        if (power > v / 3u)                    /* next power would exceed v */
            break;
        power *= 3u;
    }

    return count;
}
