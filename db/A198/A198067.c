/*
 * A198067  Square array read by antidiagonals, n>=1, k>=1; T(n,k) is the
 *          number of nonprime numbers which are prime to n and are not strong
 *          divisors of k.
 * https://oeis.org/A198067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array T(n,k) along
 *            antidiagonals * in the order
 *            (1,1),(2,1),(1,2),(3,1),(2,2),(1,3),(4,1),(3,2),(2,3),(1,4),...
 *            * For a given index n we find the diagonal sum m such that
 *            (m-1)*m/2 >= n. * Let prev = (m-1)*(m-2)/2; offset = n - prev -
 *            1. * Then the correspon...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   ugcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/ugcd_u64.h"

/* Simple deterministic primality test for small numbers (≤ 1e6). */
static inline bool is_prime_idx(idx_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (idx_t d = 3ULL; d * d <= n; d += 2ULL)
        if (n % d == 0ULL) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* Determine diagonal sum m such that (m-1)*m/2 >= n. */
    idx_t m = 1;
    while ((m - 1) * m / 2 < n) ++m;

    /* Count of elements before this diagonal. */
    idx_t prev = (m - 1) * (m - 2) / 2;
    /* Zero‑based position within the diagonal. */
    idx_t offset = n - prev - 1;

    /* Map index to array coordinates (i,j). */
    idx_t i = m - offset - 1;   /* row index (>=1) */
    idx_t j = offset + 1;       /* column index (>=1) */

    /* Count nonprime numbers coprime to i that are not strong divisors of j. */
    seq_t cnt = 0;
    for (idx_t d = 1; d <= i; ++d) {
        if (ugcd_u64(d, i) != 1) continue;          /* not coprime to i */
        if (d == 1) { ++cnt; continue; }            /* 1 is always counted */
        if (is_prime_idx(d)) continue;              /* skip primes */
        if (j % d == 0) continue;                  /* strong divisor of k */
        ++cnt;
    }
    return cnt;
}
