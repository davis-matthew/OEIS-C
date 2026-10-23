/*
 * A176203  Triangle read by rows: T(n, k) = 16*binomial(n, k) - 15.
 * https://oeis.org/A176203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n, k) = 16 * binomial(n, k) - 15, read by rows
 * Range:     u8 n <= 7, u16 n <= 125, u32 n <= 510, u64 n <= 2108,
 *            u128 n <= 5150, big any n
 * Helpers:   floor_sqrt, safe_binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/safe_binomial.h"

/* Return the row number r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static inline idx_t row_of(idx_t n)
{
    /* 8*n + 1 fits in 128 bits for n <= 10000 */
    seq_t val = (seq_t)(8 * (unsigned __int128)n + 1);
    seq_t s = floor_sqrt(val);
    return (idx_t)((s - 1) / 2);
}

seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t offset = (r * (r + 1)) / 2;
    idx_t k = n - offset;
    seq_t bin = safe_binomial(r, k);
    unsigned __int128 tmp = (unsigned __int128)bin * 16u - 15u;
    return (seq_t)tmp;
}
