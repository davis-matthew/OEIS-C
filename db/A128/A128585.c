/*
 * A128585  Triangle read by rows: A007318^(-1) * A128541.
 * https://oeis.org/A128585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = Fib(j) * (-1)^{r-j} * (C(r,j) - 2*C(r-1,j)) where r = the
 *            row number such that r*(r+1)/2 ≥ n, and j = n - r*(r-1)/2 (the
 *            column within that row)
 * Range:     i8 n <= 41, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

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
