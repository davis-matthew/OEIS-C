/*
 * A152201  Triangle read by rows, A000012 * A152198.
 * https://oeis.org/A152201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) follows from the partial sums of columns of the triangle *
 *            A152198 with entries A152198(i,j) = C(floor(i/2), j). For a row
 *            r * (0‑based) with floor(r/2) = m, the j‑th entry (0 ≤ j ≤ m) is
 *            * – if r is odd (r = 2m+1): a(r,j) = 2 * C(m+1, j+1) * – if r is
 *            even (r = 2m): a(r,j) = 2 * C(m, j+1) +...
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 48 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static inline idx_t prefix_rows(idx_t r)
{
    idx_t m = r >> 1;               /* floor(r/2) */
    if ((r & 1) == 0)               /* r even: r = 2m */
        return (m + 1) * (m + 1);
    else                            /* r odd: r = 2m+1 */
        return (m + 1) * (m + 2);
}

seq_t a(idx_t n)
{
    idx_t r = 0;
    idx_t prev_prefix = 0;

    /* Find the row containing index n. */
    while (prefix_rows(r) <= n) {
        prev_prefix = prefix_rows(r);
        ++r;
    }

    /* Column index within the found row. */
    idx_t j = n - prev_prefix;

    idx_t m = r >> 1;               /* floor(r/2) */

    if (r & 1) {                    /* odd row: r = 2m+1 */
        seq_t val = binomial(m + 1, j + 1);
        return (seq_t)2 * val;
    } else {                        /* even row: r = 2m */
        seq_t term1 = binomial(m, j + 1);  /* may be 0 for j = m */
        seq_t term2 = binomial(m, j);
        return (seq_t)2 * term1 + term2;
    }
}
