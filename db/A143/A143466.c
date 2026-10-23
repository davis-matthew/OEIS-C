/*
 * A143466  Odious count triangle, T(n,k) = A010060(n) * A010060(k); 1 <= k <=
 *          n.
 * https://oeis.org/A143466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the flattened entry of the odious
 * count triangle.  For a linear index n (1‑based) let
 *    r = smallest integer with r(r+1)/2 >= n        (row index)
 *    c = n - r(r-1)/2                                (column index)
 * Then a(n) = A010060(r) * A010060(c) where
 *    A010060(i) = popcount(i) mod 2  (Thue‑Morse bit).
 * So a(n) is 1 iff both r and c have odd popcount.   */
#include <stdint.h>
#include <math.h>

/* Build uses the type definitions for seq_t and idx_t.  */
seq_t a(idx_t n)
{
    /*  Compute row index r */
    if (n == 0) return (seq_t)0;           /* undefined below offset, return 0 */
    /* Use double to get an approximate sqrt, then correct */
    long double d = sqrt((long double)(8LL * n - 1LL));
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);
    /* Correct possible off‑by‑one error */
    while (r * (r + 1) / 2 < n) ++r;

    /* Column index within the row */
    idx_t prev = (r - 1) * r / 2;         /* entries before this row */
    idx_t c = n - prev;                   /* 1‑based column */

    /* Thue-Morse bit is 1 iff popcount is odd */
    uint64_t pr = __builtin_popcountll(r);
    uint64_t pc = __builtin_popcountll(c);

    return (seq_t)((pr & 1ULL) && (pc & 1ULL));
}
