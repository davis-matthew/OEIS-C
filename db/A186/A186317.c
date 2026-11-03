/*
 * A186317  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f and g are the squares and hexagonal
 *          numbers. Complement of A186318.
 * https://oeis.org/A186317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((1 + sqrt(1 + 8*n^2)) / 4)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (rank of the n-th square among the ordered list of squares
    and hexagonal numbers, with ties resolved by placing the
    hexagonal number before the square) */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) of a non‑negative seq_t value. */
static seq_t isqrt_seq(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* for n < 1 the behaviour is irrelevant */
    if (n == 0)
        return 0;

    seq_t ns = (seq_t)n;                /* n as seq_t */
    seq_t n2 = ns * ns;                 /* n^2 */
    seq_t t  = (seq_t)1 + ((seq_t)8 * n2); /* 1 + 8*n^2 */

    seq_t sqrt_t = isqrt_seq(t);        /* floor(sqrt(t)) */
    seq_t hexCnt = ((seq_t)1 + sqrt_t) / (seq_t)4; /* number of hexagons ≤ n^2 */

    return ns + hexCnt;                 /* rank of the square n^2 */
}
