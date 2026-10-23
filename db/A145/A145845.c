/*
 * A145845  Number of permutations of length 2n+1 which are invariant under
 *          the reverse-complement map and have no decreasing subsequences of
 *          length 5.
 * https://oeis.org/A145845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   alloc_seq, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (n+2)^3*(3*n+1)*a(n) =
     2*(30*n^4 + 67*n^3 + 29*n^2 - 10*n - 8)*a(n-1)
     - 64*(n-1)^2*n*(3*n+4)*a(n-2)
   Initial values: a(0)=1, a(1)=2 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq.h"
#include "../utils/grow_seq.h"

#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 big_t;
#else
typedef seq_t big_t;
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached values of a(0..k) */
    static idx_t cache_size = 0;  /* number of values stored (next free index) */
    static size_t cache_cap  = 0;  /* allocated capacity */

    /* initialise cache with the first two terms on first call */
    if (!cache) {
        cache_cap  = 8;
        cache      = alloc_seq(cache_cap);
        cache[0]   = (seq_t)1;
        cache[1]   = (seq_t)2;
        cache_size = 2;
    }

    /* compute any missing terms up to n */
    if (n >= cache_size) {
        /* enlarge cache to hold at least n+1 values */
        grow_seq(&cache, &cache_cap, n + 1);

        for (idx_t m = cache_size; m <= n; ++m) {
            /* convert index to the arithmetic type used for the recurrence */
            big_t m_seq  = (big_t)m;
            big_t one    = (big_t)1;
            big_t two    = (big_t)2;
            big_t three  = (big_t)3;
            big_t four   = (big_t)4;
            big_t six    = (big_t)6;
            big_t eight  = (big_t)8;
            big_t ten    = (big_t)10;
            big_t thirty = (big_t)30;
            big_t sixtyseven = (big_t)67;
            big_t twentynine  = (big_t)29;
            big_t three_m_plus1, m_plus2, m_minus1, three_m_plus4;
            big_t poly;

            m_plus2       = m_seq + two;
            three_m_plus1 = three * m_seq + one;
            m_minus1      = m_seq - one;
            three_m_plus4 = three * m_seq + four;

            /* coefficient on the left side of the recurrence */
            big_t coeff = m_plus2 * m_plus2 * m_plus2 * three_m_plus1;

            /* polynomial part: 30*m^4 + 67*m^3 + 29*m^2 - 10*m - 8 */
            big_t m2 = m_seq * m_seq;          /* m^2 */
            big_t m3 = m2 * m_seq;             /* m^3 */
            big_t m4 = m3 * m_seq;             /* m^4 */
            poly = thirty * m4 + sixtyseven * m3 + twentynine * m2 - ten * m_seq - eight;

            /* term1 = 2 * poly * a(m-1) */
            big_t term1 = two * poly * ((big_t)cache[m - 1]);

            /* term2 = 64 * (m-1)^2 * m * (3*m+4) * a(m-2) */
            big_t m_minus1_sq = m_minus1 * m_minus1;
            big_t term2 = (big_t)64 * m_minus1_sq * m_seq * three_m_plus4 * ((big_t)cache[m - 2]);

            /* RHS of the recurrence */
            big_t rhs = term1 - term2;

            /* a(m) = rhs / coeff (exact division) */
            cache[m] = (seq_t)(rhs / coeff);
        }

        cache_size = n + 1;
    }

    return cache[n];
}
