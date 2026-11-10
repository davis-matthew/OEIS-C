/*
 * A067586  Number of 0's in the binary expansion of A066884(n+1).
 * https://oeis.org/A067586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, tabl, nonn
 * Formula:   a(n) = A023416(A066884(n+1)); computed as (bit length of
 *            A066884(n+1)) - (number of ones), via combinatorial unranking of
 *            the top bit
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* C(n,k), returning limit+1 if the value exceeds limit */
static idx_t A067586_comb_le(idx_t n, idx_t k, idx_t limit)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return (1 <= limit) ? 1 : limit + 1;
    if (k > n - k) k = n - k;
    idx_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        unsigned __int128 tmp = (unsigned __int128)result * (n - k + i);
        tmp /= i;
        if (tmp > limit) return limit + 1;
        result = (idx_t)tmp;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t m = n + 1;
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 < m) ++d;
    idx_t prev_total = d * (d + 1) / 2;
    idx_t offset = m - prev_total - 1;
    idx_t r = d + 1 - offset;
    idx_t rank = offset;

    /* top bit position p: largest p with C(p,r) <= rank */
    idx_t p = r - 1;
    while (A067586_comb_le(p + 1, r, rank) <= rank) ++p;

    return (seq_t)(p + 1 - r);
}
