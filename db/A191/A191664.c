/*
 * A191664  Dispersion of A014601 (numbers >2, congruent to 0 or 3 mod 4), by
 *          antidiagonals.
 * https://oeis.org/A191664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) is obtained by traversing an infinite 2‑D array M whose
 *            rows * are defined as follows: for row r ≥ 1 let * c = r if r is
 *            odd * c = r – 1 if r is even (c = 1,1,3,3,5,5,…) * * For column
 *            j ≥ 1 * M[r][j] = c * 2^j – (r odd ? 1 : 0) * * The sequence
 *            a(n) lists the elements of M in anti‑diagona...
 * Range:     u8 n <= 36, u16 n <= 136, u32 n <= 528, u64 n <= 2080,
 *            u128 n <= 5050, big any n
 * Helpers:   pow2, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/seq_one.h"

/* integer square root of a 128‑bit unsigned integer, returning floor(sqrt(x)) */
static unsigned long long isqrt128(unsigned __int128 x)
{
    unsigned long long r = 0;
    unsigned long long bit = 1ULL << 62;
    while (bit > x) bit >>= 2;
    while (bit)
    {
        unsigned long long tmp = r + bit;
        if ((unsigned __int128)tmp * tmp <= x) r = tmp;
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return seq_one();            /* undefined for offset 0, return 1 */

    /* 1. Find the anti‑diagonal number d such that d(d+1)/2 ≥ n  */
    idx_t lo = 0, hi = 1;
    while ( ((unsigned __int128)hi * (hi + 1) / 2) < n )
        hi <<= 1;
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 tri = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (tri >= n) hi = mid;
        else          lo = mid + 1;
    }
    idx_t d = lo;                            /* diagonal index (>=1) */

    /* 2. Position inside the diagonal */
    unsigned __int128 prev_tri = ((unsigned __int128)d * (d - 1)) / 2;
    idx_t k = (idx_t)(n - prev_tri);         /* 1‑based index inside diagonal */

    /* 3. Coordinates: row r = k, column j = d - k + 1 */
    idx_t r = k;
    idx_t j = d - k + 1;

    /* 4. Compute coefficient c */
    idx_t c = (r & 1) ? r : r - 1;           /* odd rows: r, even rows: r-1 */

    /* 5. Compute the value */
    seq_t power = pow2(j);                   /* 2^j */
    seq_t val = power * (seq_t)c;            /* c * 2^j */

    if (r & 1)                               /* subtract 1 for odd rows */
        val = val - seq_one();

    return val;
}
