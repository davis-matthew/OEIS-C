/*
 * A103820  Whitney transform of 3^n.
 * https://oeis.org/A103820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-3); a(0)=1, a(1)=4, a(2)=16
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* memoization storage for computed terms */
static seq_t *memo = NULL;
static idx_t memo_len = 0;      /* allocated length of memo array   */
static idx_t max_computed = 0;  /* index of the largest computed term */

/* ensures memo has space for indices up to 'need' (inclusive) */
static void ensure_capacity(idx_t need)
{
    if (need < memo_len)
        return;
    /* grow array exponentially to reduce reallocations */
    idx_t new_len = memo_len ? memo_len << 1 : 1;
    while (new_len <= need)
        new_len <<= 1;
    seq_t *new_mem = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
    if (!new_mem)
        abort();           /* out of memory; terminate for safety */
    memo = new_mem;
    memo_len = new_len;
}

/* compute A103820 at index n */
seq_t a(idx_t n)
{
    /* initialize base terms on first call */
    if (!memo) {
        memo = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!memo)
            abort();          /* out of memory */
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)4;
        memo[2] = (seq_t)16;
        memo_len = 3;
        max_computed = 2;
    }

    if (n <= max_computed)
        return memo[n];

    ensure_capacity(n);

    /* compute missing terms up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t term = 4 * memo[k - 1];
        term -= 3 * memo[k - 3];
        memo[k] = term;
    }
    max_computed = n;
    return memo[n];
}
