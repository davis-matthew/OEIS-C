/*
 * A054491  a(n) = 4*a(n-1) - a(n-2), a(0)=1, a(1)=6.
 * https://oeis.org/A054491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2), a(0)=1, a(1)=6
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised computation of the recurrence */
static seq_t *table = NULL;      /* array holding computed terms   */
static idx_t  cache_len = 0;     /* number of valid entries in table */
static idx_t  cache_cap = 0;     /* allocated size of table        */

/* extend the table to hold at least 'need' entries (need ≤ cache_cap), 0‑based */
static void grow_cache(idx_t need)
{
    if (cache_cap >= need)
        return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *new_table = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
    table = new_table;
    cache_cap = new_cap;
}

/* initialise the table with the two seed values */
static void init_table(void)
{
    if (table)
        return;
    cache_cap = 2;
    table = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    table[0] = (seq_t)1;
    table[1] = (seq_t)6;
    cache_len = 2;
}

seq_t a(idx_t n)
{
    init_table();

    /* result is already known */
    if (n < cache_len)
        return table[n];

    /* grow the cache to hold the required index + 1 */
    grow_cache(n + 1);

    /* compute the missing terms iteratively */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) = 4*a(i-1) - a(i-2) */
        seq_t prev  = table[i - 1];
        seq_t prev2 = table[i - 2];
        table[i] = prev * (seq_t)4 - prev2;
    }

    cache_len = n + 1;
    return table[n];
}
