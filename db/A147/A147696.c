/*
 * A147696  Triangle read by rows: numbers n and columns k such that T(n, k)
 *          is n mod k.
 * https://oeis.org/A147696
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is obtained by flattening the triangle * T(r,k) = r mod k
 *            where the rows r ≥ 2 contain the * columns k = 2 …
 *            floor(sqrt(r))+1. * The flattened sequence starts with a(2)
 *            (offset 2)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Integer square‑root for idx_t (64‑bit).  Returns floor(sqrt(n)). */
static inline idx_t
isqrt_u64(idx_t n)
{
    /* Initial estimate using double. */
    idx_t x = (idx_t) sqrt((double) n);
    /* Correct any possible over‑ or under‑shoot. */
    while (x + 1 <= n / (x + 1)) ++x;
    while (x > n / x) --x;
    return x;
}

seq_t
a(idx_t n)
{
    /* For indices below the offset the values are undefined, return 0. */
    if (n < 2) return (seq_t)0;

    /* Pos is the 0‑based position in the flattened triangle. */
    idx_t pos = n - 2;

    /* Find the row r where this position lies.
     * Each row r contributes floor(sqrt(r)) entries (columns 2…√r+1). */
    idx_t r   = 2;
    idx_t tot = 0;                 /* total entries seen so far (for rows < r). */
    while (1) {
        idx_t cnt = isqrt_u64(r);  /* number of entries in row r. */
        if (tot + cnt > pos)      /* the required entry is in this row. */
            break;
        tot += cnt;
        ++r;
    }

    /* Offset within the row. */
    idx_t offset = pos - tot;                 /* 0‑based index in the row. */
    idx_t k = 2 + offset;                     /* column index. */
    return (seq_t) r % (seq_t) k;             /* T(r,k) = r mod k. */
}
