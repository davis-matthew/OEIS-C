/*
 * A166373  Triangle read by rows for floor(j^2 / n) with n >= 2 and 1<=j<n.
 * https://oeis.org/A166373
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = floor( j^2 / r ) where r ≥ 2 is the unique integer
 *            satisfying (r−1)·r/2 > n−2 and j = (n−2)−(r−2)(r−1)/2 + 1 (1 ≤ j
 *            < r)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for __uint128_t */

static idx_t sqrt128( __uint128_t v )
{
    /* binary‑search integer square root for 128‑bit values */
    idx_t r   = 0;
    idx_t bit = (idx_t)1 << 63;          /* start from the highest bit */
    while (bit > 0)
    {
        idx_t temp = r | bit;
        __uint128_t temp2 = (__uint128_t)temp * (__uint128_t)temp;
        if (temp2 <= v)
            r = temp;
        bit >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;            /* offset of the sequence */

    /* position within the flattened array (0‑based, after a(2)) */
    idx_t p = n - 2;

    /* find r:  (r-1)*r/2 > p */
    __uint128_t v   = (__uint128_t)8 * p + 1;     /* 1 + 8·p  */
    idx_t      s    = sqrt128(v);                 /* floor sqrt(1+8p) */
    idx_t      r    = (s + 1) / 2 + 1;             /* r ≥ 2 */

    /* start index of row r (number of elements before this row) */
    __uint128_t start_cum128 = (__uint128_t)(r-2) * (r-1);
    idx_t      start_cum = (idx_t)(start_cum128 / 2);

    /* j is 1‑based column inside the row */
    idx_t      j = (p - start_cum) + 1;          /* 1 ≤ j < r */

    /* compute floor(j^2 / r) */
    seq_t val = (seq_t)j * (seq_t)j / (seq_t)r;
    return val;
}
