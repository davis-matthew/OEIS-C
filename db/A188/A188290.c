/*
 * A188290  Positions of 0 in the zero-one sequence [nr+4r]-[nr]-[4r], where
 *          r=sqrt(5) and []=floor.
 * https://oeis.org/A188290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the index of the n‑th 0 in the zero‑one sequence *
 *            floor(k*sqrt(5)+4*sqrt(5)) - floor(k*sqrt(5)) -
 *            floor(4*sqrt(5)), * where floor(4*sqrt(5)) = 8. The difference
 *            is 8 or 9; zeros * correspond to the case 8
 * Range:     u8 n <= 14, u16 n <= 3652, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Integer square root of an unsigned 128‑bit integer.
 * Returns floor(sqrt(x)).  Binary search over 0..2^64‑1. */
static inline uint64_t isqrt_uint128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 64) - 1;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return lo - 1;
}

/* Compute the position of the n‑th zero.  Stores the zero positions
 * in a static array of idx_t locations to avoid allocating big
 * integer objects.  Only idx_t arithmetic is needed; when the
 * result is returned it is cast to seq_t. */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for n < 1 */
        return 0;

    /* static cache of zero positions (as idx_t for portability) */
    static idx_t *pos = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t sz    = 0;   /* number of stored zero positions */
    static idx_t  last_k = 0;  /* last integer k examined */

    /* grow cache as needed */
    while (sz < (size_t)n) {
        ++last_k;                 /* next integer to test */
        idx_t k = last_k;
        idx_t k4 = k + 4ULL;

        /* compute floor(k*sqrt5) via floor(sqrt(5*k^2))   */
        unsigned __int128 t2 = (unsigned __int128)5 * (unsigned __int128)k * (unsigned __int128)k;
        unsigned __int128 t1 = (unsigned __int128)5 * (unsigned __int128)k4 * (unsigned __int128)k4;

        uint64_t v2 = isqrt_uint128(t2);
        uint64_t v1 = isqrt_uint128(t1);

        /* difference is 8 or 9; zeros correspond to 8 */
        if (v1 - v2 == 8ULL) {
            /* ensure capacity of cache array */
            if (sz == cap) {
                size_t newcap = cap ? cap * 2 : 16;
                idx_t *tmp = (idx_t*)realloc(pos, newcap * sizeof(idx_t));
                if (!tmp) abort();            /* out‑of‑memory */
                pos = tmp;
                cap = newcap;
            }
            pos[sz++] = (idx_t)last_k;    /* store the zero position */
        }
    }

    /* cast the stored idx_t position to the required seq_t type */
    return (seq_t)pos[n - 1];
}
