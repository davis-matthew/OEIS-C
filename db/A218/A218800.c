/*
 * A218800  Number of nonnegative integer solutions to x^2 + 2y^2 = (3n)^2.
 * https://oeis.org/A218800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of nonnegative integer solutions of x^2 + 2y^2 = (3n)^2
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: integer square root of v with upper bound max_val (non‑negative result) */
static inline seq_t isqrt_seq(seq_t v, seq_t max_val)
{
    seq_t lo = 0;
    seq_t hi = max_val;
    while (lo < hi) {
        seq_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid != 0 && mid > v / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/*  a(n) : number of nonnegative integer solutions to x^2 + 2y^2 = (3n)^2  */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;           /* convert index to seq_t */
    seq_t M = (seq_t)3 * N;       /* right side m = 3n   */
    seq_t M2 = M * M;             /* m^2                 */
    seq_t count = 0;

    for (idx_t y = 0; ; ++y) {
        seq_t yy   = (seq_t)y;
        seq_t prod = (seq_t)2 * yy * yy;   /* 2*y^2           */

        /* stop when 2*y^2 exceeds m^2 */
        if (M2 < prod) break;

        seq_t val = M2 - prod;            /* remaining square to test */

        /* test if val is a perfect square */
        seq_t x = isqrt_seq(val, M);      /* x <= m           */
        bool is_square;
        if (x == 0)
            is_square = (val == 0);
        else {
            seq_t div = val / x;
            seq_t rem = val % x;
            is_square = (div == x) && (rem == 0);
        }
        if (is_square)
            ++count;
    }
    return count;
}
