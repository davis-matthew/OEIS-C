/*
 * A098236  Number of ways to write n as the sum of two positive distinct
 *          squarefree numbers.
 * https://oeis.org/A098236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..⌊(n-1)/2⌋} μ(k)^2 * μ(n-k)^2 (count of pairs of
 *            distinct positive square‑free integers that sum to n)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static state for the square‑free table */
static idx_t    sqfree_max = 0;
static uint8_t *sqfree_tbl = NULL;

/* integer square root (floor) for 64‑bit values */
static idx_t int_isqrt(idx_t v) {
    idx_t r = 0;
    /* small loop, sqrt(2^64) < 2^32 */
    while (((r + 1) * (r + 1)) <= v)
        ++r;
    return r;
}

/* ensure sqfree_tbl contains square‑free information for all integers
   1 .. n (sqfree_tbl[0] is unused).  The array is expanded as needed. */
static void ensure_sqfree(idx_t n) {
    if (n <= sqfree_max)
        return;

    /* enlarge the table */
    size_t newlen = (size_t)(n + 1);
    uint8_t *tmp = (uint8_t *)realloc(sqfree_tbl, newlen);
    if (!tmp)
        abort();            /* out of memory */
    sqfree_tbl = tmp;

    /* initialize new entries as square‑free (value 1) */
    for (idx_t i = sqfree_max + 1; i <= n; ++i)
        sqfree_tbl[i] = 1;

    /* run a simple square sieve up to n */
    idx_t lim = int_isqrt(n);
    for (idx_t i = 2; i <= lim; ++i) {
        idx_t sq = i * i;
        for (idx_t j = sq; j <= n; j += sq)
            sqfree_tbl[j] = 0;
    }

    sqfree_max = n;
}

seq_t a(idx_t n) {
    if (n < 1)
        return 0;

    ensure_sqfree(n);

    const idx_t limit = (n - 1) / 2;
    seq_t cnt = 0;
    for (idx_t k = 1; k <= limit; ++k) {
        if (sqfree_tbl[k] && sqfree_tbl[n - k])
            ++cnt;
    }
    return cnt;
}
