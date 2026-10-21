/*
 * A064863  Period of continued fraction for sqrt(11)*n.
 * https://oeis.org/A064863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = period of the regular continued fraction for sqrt(11*n^2)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   uint128_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently, the period of the continued fraction for sqrt(11)*n)          */

/* The algorithm follows the standard “simple continued fraction for sqrt(D)”
 * with D = 11*n^2.  All arithmetic is performed in 128‑bit integers to
 * avoid overflow for the range of n used in the tests.  The period is an
 * integer that fits in a 64‑bit type, so it is returned as a seq_t.       */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/uint128_isqrt.h"

/* Return the period of the continued fraction of sqrt(11*n^2).           */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n=0 – return 0 */

    /* D = 11 * n^2 (128‑bit).                               */
    unsigned __int128 nn  = (unsigned __int128)n;
    unsigned __int128 D   = 11ULL * nn * nn;

    /* a0 = floor(sqrt(D)) (128‑bit).                          */
    unsigned __int128 a0  = uint128_isqrt(D);

    /* continued‑fraction variables (128‑bit). */
    unsigned __int128 m    = 0;
    unsigned __int128 d    = 1;
    unsigned __int128 a_cur = a0;
    unsigned __int128 two_a0 = a0 + a0;

    seq_t period = 0;              /* period counter */

    while (true) {
        /* m = d*a_cur - m */
        m = d * a_cur - m;

        /* d = (D - m^2) / d   (exact division). */
        unsigned __int128 m_sq  = m * m;
        unsigned __int128 diff  = D - m_sq;
        d = diff / d;

        /* a_cur = floor((a0 + m) / d) */
        a_cur = (a0 + m) / d;

        period++;

        /* Period detection: simple continued fraction for sqrt(D)
         * ends when a_cur == 2*a0.                           */
        if (a_cur == two_a0)
            break;
    }

    return period;
}
