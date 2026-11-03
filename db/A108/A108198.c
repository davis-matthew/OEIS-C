/*
 * A108198  Triangle read by rows: T(n,k) =
 *          binomial(2k+2,k+1)*binomial(n,k)/(k+2) (0 <= k <= n).
 * https://oeis.org/A108198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = binomial(2k+2,k+1) * binomial(n,k) / (k+2); a(n) is the
 *            triangle read by rows
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 57 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <sys/types.h>
#include <stdint.h>
#include "../utils/binomial.h"

/* Catalan number C(k) = binom(2k,k)/(k+1) computed multiplicatively */
static seq_t catalan(idx_t k)
{
    seq_t res = (seq_t)1;
    /* compute binom(2k,k) */
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        res = res * (seq_t)(k + i) / (seq_t)i;
    }
    /* divide by (k+1) to get Catalan number */
    res /= (seq_t)(k + 1);
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) {
        r++;
    }
    /* Column index within the row. */
    idx_t k = n - (r * (r + 1)) / 2;

    /* T(r,k) = Catalan(k+1) * binomial(r,k). */
    seq_t val = catalan(k + 1);
    val *= binomial(r, k);
    return val;
}
