/*
 * A128585  Triangle read by rows: A007318^(-1) * A128541.
 * https://oeis.org/A128585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = Fib(j) * (-1)^{r-j} * (C(r,j) - 2*C(r-1,j)) where r = the
 *            row number such that r*(r+1)/2 ≥ n, and j = n - r*(r-1)/2 (the
 *            column within that row)
 * Range:     i8 n <= 30, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   binomial, fib, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* for binomial(idx_t, idx_t)          */
#include "../utils/fib.h"        /* for fib(idx_t)                      */
#include "../utils/mul_small.h"  /* for mul_small(seq_t, unsigned int) */

seq_t a(idx_t n) {
    /* Find the row r that contains the nth element (1‑based). */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Column index j within the row (1‑based). */
    idx_t start = r * (r - 1) / 2;   /* first linear index of the row */
    idx_t j = n - start;             /* column number, 1 ≤ j ≤ r */

    /* Compute binomial coefficients. */
    seq_t C1 = binomial(r, j);           /* C(r,j)          */
    seq_t C2 = 0;
    if (j <= r - 1)                      /* C(r-1,j) if valid */
        C2 = binomial(r - 1, j);

    /* Compute (C(r,j) - 2*C(r-1,j)). */
    seq_t twoC2 = mul_small(C2, 2);      /* multiplies C2 by 2 */
    seq_t diff   = C1 - twoC2;           /* C(r,j) - 2*C(r-1,j) */

    /* Multiply by Fib(j). */
    seq_t fibj   = fib(j);
    seq_t prod   = diff * fibj;          /* uses integer multiplication */

    /* Apply the sign factor (-1)^{r-j}. */
    if ( ((r - j) & 1) != 0 )
        prod = -prod;                    /* unary minus is allowed on seq_t */

    return prod;
}
