/*
 * A143085  Triangle read by rows: T(n,k) = (n + 1)*(n + k)!.
 * https://oeis.org/A143085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r+1)*(r + (n - r*(r+1)/2))! where r =
 *            floor((sqrt(8n+1)-1)/2)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 33, u64 n <= 64, u128 n <= 169,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"

/* Integer square root of a 128‑bit unsigned integer */
static idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)~(idx_t)0;          /* max 64‑bit value */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;                           /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* r = floor((sqrt(8n+1)-1)/2) */
    unsigned __int128 t = (unsigned __int128)8 * n + 1;
    idx_t r = (isqrt_u128(t) - 1) / 2;

    /* number of terms before row r */
    idx_t sumPrev = (idx_t)((unsigned __int128)r * (r + 1) / 2);

    /* column index within the row */
    idx_t k = n - sumPrev;

    /* factorial index */
    idx_t factIndex = r + k;

    seq_t fact = factorial(factIndex);
    seq_t result = (seq_t)(r + 1) * fact;

    return result;
}
