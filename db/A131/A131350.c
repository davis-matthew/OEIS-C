/*
 * A131350  2*A007318 - A049310 as infinite lower triangular matrices.
 * https://oeis.org/A131350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A131350
 *
 *  The entry in row `r` and column `c` (0‑based) is
 *
 *      B(r,c) = 2 * C(r,c) – D(r,c)
 *
 *  where C(r,c) is the binomial coefficient “r choose c” and
 *
 *      D(r,c) = 0                                         if r and c have opposite parity
 *             = C( (r/2)+(c/2)      , c )     when r and c are even
 *             = C( (r+1)/2 + (c+1)/2 – 1 , c )  when r and c are odd
 *
 *  The index `n` is converted to `(r,c)` by the usual triangle
 *  mapping:  n = r*(r+1)/2 + c  (0 ≤ c ≤ r).
 *
 *  The function below implements this definition using only integer
 *  arithmetic on the provided type `seq_t` (which may be 8–128 bits
 *  or arbitrary‑precision).  No floating‑point operations are used on
 *  sequence values.
 */

#include <stddef.h>

static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;        /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(n - k + i));
        res = res / ((seq_t)i);
    }
    return res;
}

seq_t a(idx_t n)
{
    /* find the row r with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) r++;

    idx_t base = r * (r + 1) / 2;
    idx_t c = n - base;                         /* column index */

    seq_t binom_rc = binom_seq(r, c);

    seq_t D;
    if ((r & 1) != (c & 1)) {
        D = 0;                                   /* parity mismatch */
    } else if ((r & 1) == 0) {                   /* both even */
        idx_t a = r >> 1;
        idx_t b = c >> 1;
        D = binom_seq(a + b, c);                 /* C( a+b , c ) */
    } else {                                     /* both odd */
        idx_t a = (r + 1) >> 1;
        idx_t b = (c + 1) >> 1;
        D = binom_seq(a + b - 1, c);             /* C( a+b-1 , c ) */
    }

    return (binom_rc << 1) - D;                  /* 2*C(r,c) – D */
}
