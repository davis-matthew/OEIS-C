/*
 * A127172  Triangle read by rows: A051731^3 as an infinite lower triangular
 *          matrix.
 * https://oeis.org/A127172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the lower‑triangular matrix defined
 *            by T(r,c) = A007425(r/c) if c|r, else 0, row by row. A007425 is
 *            multiplicative with A007425(p^e) = C(e+2,2) = (e+2)(e+1)/2,
 *            where p is prime and e≥1. For n=1, A007425(1)=1
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   seq_A007425
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_A007425.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* sequence is 1-indexed */

    /* Find row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    idx_t accum = 1;                 /* sum of first r integers */
    while (accum < n) {
        ++r;
        accum += r;
    }

    /* Compute column (1‑based) within the row */
    idx_t row_start = r * (r - 1) / 2 + 1;
    idx_t offset = n - row_start;    /* 0‑based offset in the row */
    idx_t c = offset + 1;

    if (r % c != 0) return (seq_t)0; /* not a divisor */

    idx_t m = r / c;
    return seq_A007425(m);
}
