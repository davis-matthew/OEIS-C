/*
 * A134434  Triangle read by rows: T(n,k) is the number of permutations of
 *          {1,2,...,n} having k even entries that are followed by a smaller
 *          entry (n>=0, k>=0).
 * https://oeis.org/A134434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattened triangle T(r,k) where T(2m,k) = (m! *
 *            C(m,k))^2 T(2m+1,k) = ((m+1)! * C(m,k))^2 / (k+1) 0 ≤ k ≤
 *            floor(r/2)
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 57, u64 n <= 134,
 *            u128 n <= 328, big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS sequence A134434. */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t r = 0;          /* current row index */
    idx_t cum = 0;        /* total elements before this row */
    idx_t len;

    /* Find the row r such that the n‑th element lies in it. */
    while (1) {
        len = (r >> 1) + 1;          /* number of entries in row r */
        if (cum + len > n)
            break;                  /* n is inside this row */
        cum += len;
        r++;
    }

    idx_t k = n - cum;               /* column index within row r */
    idx_t m = r >> 1;                /* floor(r/2) */

    if ((r & 1) == 0) {              /* even row: r = 2m */
        seq_t fac = factorial(m);
        seq_t bin = binomial(m, k);
        seq_t prod = fac * bin;
        return prod * prod;
    } else {                         /* odd row: r = 2m+1 */
        seq_t fac = factorial(m + 1);
        seq_t bin = binomial(m, k);
        seq_t prod = fac * bin;
        seq_t sq   = prod * prod;
        return sq / ((seq_t)(k + 1));
    }
}
