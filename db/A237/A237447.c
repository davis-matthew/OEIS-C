/*
 * A237447  Infinite square array: row 1 is the positive integers 1, 2, 3,
 *          ..., and on any subsequent row n, n is moved to the front: n, 1,
 *          ..., n-1, n+1, n+2, ...
 * https://oeis.org/A237447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8830, u16 n <= 8830, u32 n <= 8830, u64 n <= 8830,
 *            u128 n <= 8830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A237447: row n of the infinite table where row 1 is 1,2,3,...,
   and on row n the integer n is placed at the front.
   The value a(n) corresponds to the (row,col) pair obtained by
   traversing the table anti‑diagonally and applying
   T(row,col) = row            if col = 1
                col            if col > row
                col-1          otherwise. */
#include <stdint.h>

static idx_t isqrt_u128(__int128 x)
/* Integer square root: largest s with s*s <= x */
{
    idx_t r = 0;
    for (int i = 63; i >= 0; --i) {
        idx_t t = r | ((idx_t)1 << i);
        if ((__int128)t * t <= x)
            r = t;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)        /* sequence is 1‑based; undefined for 0 */
        return 0;

    /* Find diagonal s such that (s-1)(s-2)/2 < n <= s(s-1)/2. */
    __int128 t = (__int128)8 * n + 1;               /* 1 + 8*n  */
    idx_t sqrt_t = isqrt_u128(t);                   /* floor(sqrt(t)) */
    idx_t s = (1 + sqrt_t) / 2;                    /* initial guess */

    __int128 diag = (__int128)s * (s - 1) / 2;
    if (diag < n)      /* adjust upward if necessary */
        ++s;

    /* Count of terms before this diagonal. */
    __int128 before = (__int128)(s - 1) * (s - 2) / 2;
    idx_t offset = (idx_t)(n - before - 1);         /* zero‑based offset in diagonal */

    idx_t r = 1 + offset;                           /* row index on this diagonal */
    idx_t c = s - r;                                /* column index (c >= 1) */

    if (c == 1)
        return (seq_t)r;
    else if (c > r)
        return (seq_t)c;
    else
        return (seq_t)(c - 1);
}
