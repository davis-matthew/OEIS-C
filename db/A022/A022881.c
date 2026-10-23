/*
 * A022881  The number of numbers [ [ ix ]jx ] that equal n, where i >= 1, j
 *          >= 1 and x=sqrt(3).
 * https://oeis.org/A022881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (i , j ≥ 1) such that * floor(floor(i·√3)
 *            · j · √3) = n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"

static const idx_t SCALE = 10000000ULL;          /* 10^7 : > maximum i*j * sqrt(3) needed */
static idx_t P = 0;                              /* floor(SCALE·√3) */

seq_t a(idx_t n)
{
    /* Lazily initialise the constant P = floor(SCALE * sqrt(3)) */
    if (!P) {
        /* 3 * SCALE^2 fits in 64‑bit (3·10^14 < 2^63) */
        P = _floor_isqrt_u64(3ULL * SCALE * SCALE);
    }

    seq_t count = 0;                            /* result counter */

    for (idx_t i = 1; ; ++i) {
        /* I = floor(i·√3)  using the integer approximation */
        idx_t I = ( ( (__int128) i * P ) / SCALE );

        /* test whether any j ≥ 1 could work for this I
         * If floor(I·√3) ≥ n+1, then I·j·√3 ≥ n+1 for all j ≥ 1
         */
        idx_t test = ( ( (__int128) I * P ) / SCALE );
        if (test >= n + 1)
            break;                               /* no further solutions possible */

        /* j must satisfy n ≤ I·j·√3 < n+1
         * Solve bounds in integers:
         * n·SCALE ≤ I·j·P < (n+1)·SCALE
         * Thus j = ceil( n·SCALE / (I·P) )
         */
        __int128 denom = ( __int128 ) I * P;
        __int128 numer = ( __int128 ) n * SCALE;
        idx_t j = (idx_t)( ( numer + denom - 1 ) / denom );   /* ceil division */

        /* Verify that this j really gives the required value */
        __int128 val128 = ( (__int128) I * j * P ) / SCALE;
        idx_t val = (idx_t) val128;

        if (val == n)
            ++count;
    }

    return count;
}
