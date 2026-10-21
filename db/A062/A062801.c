/*
 * A062801  Number of 2 X 2 non-singular integer matrices with entries from
 *          {0,...,n}.
 * https://oeis.org/A062801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   * a(n) = (n+1)^4 – Σ_{p=0}^{n^2} f(p)^2 * where f(p) is the
 *            number of ordered pairs (a,d) with 0≤a,d≤n and a·d=p. * This
 *            counts the non‑singular 2×2 integer matrices with entries in
 *            {0,…,n}
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Total number of all 2×2 matrices.  (n+1)^4 fits in uint64_t for n≤10000. */
    uint64_t np1 = (uint64_t)(n + 1);
    uint64_t total = np1 * np1 * np1 * np1;

    /* Size of the product table: all possible products a·d, 0≤a,d≤n. */
    size_t prod_max = (size_t)n * (size_t)n;
    size_t array_size = prod_max + 1;

    /* Allocate a zeroed array of 16‑bit counters. */
    uint16_t *counts = (uint16_t *)calloc(array_size, sizeof(uint16_t));
    if (!counts) return (seq_t)0;          /* Allocation failure – return 0. */

    uint64_t sum_squares = 0;  /* Σ f(p)^2   */
    for (idx_t a_val = 0; a_val <= n; ++a_val) {
        for (idx_t d_val = 0; d_val <= n; ++d_val) {
            idx_t prod = a_val * d_val;          /* 0 ≤ prod ≤ n², fits in idx_t. */
            uint16_t old_cnt = counts[prod];
            counts[prod] = old_cnt + 1;
            sum_squares += (uint64_t)(2u * old_cnt + 1);
        }
    }

    uint64_t nonsingular = total - sum_squares;
    free(counts);
    return (seq_t)nonsingular;
}
