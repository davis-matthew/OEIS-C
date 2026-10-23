/*
 * A134234  Triangle read by rows, n-th row = n terms of 2n, 2n+2, 2n+4, ...,
 *          1; with a(1) = 1.
 * https://oeis.org/A134234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) gives the n‑th element of the triangle whose r‑th row
 *            contains the terms 2r, 2r+2, …, 2r+2(r‑2), 1 (row 1 has a single
 *            element 1). Each row has r terms, the last being 1. The sequence
 *            is indexed starting at n = 1
 * Range:     u8 n <= 2143, u16 n <= 9623, u32 n <= 9623, u64 n <= 9623,
 *            u128 n <= 9623, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root (floor) for unsigned integers */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            low = 1;
            continue;
        }
        if (mid <= n / mid)
            low = mid + 1;          /* mid^2 <= n, search higher */
        else
            high = mid - 1;         /* mid^2 > n, search lower */
    }
    return high;                   /* floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    /* n >= 1; guard against zero if called erroneously */
    if (n == 0)
        return (seq_t)0;

    /* Find the row r such that (r‑1)r/2 + 1 <= n < r(r+1)/2 + 1 */
    idx_t t = 8 * (n - 1) + 1;     /* discriminant of the quadratic */
    idx_t s = isqrt_u64(t);
    idx_t r = (1 + s) / 2;         /* row number (1‑based) */

    /* Starting index of this row */
    idx_t offset = ((r - 1) * r) / 2 + 1;

    /* Position within the row (0‑based) */
    idx_t pos = n - offset;

    /* Last element of the row is always 1 */
    if (pos == r - 1)
        return (seq_t)1;

    /* Otherwise the term is 2r + 2*pos */
    seq_t val = (seq_t)(2 * r + 2 * pos);
    return val;
}
