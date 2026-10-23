/*
 * A144895  Second column of triangle A134134 (S2'(2) = S1hat(2)).
 * https://oeis.org/A144895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..⌊n/2⌋+1} i! * (n-i+2)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of factorials */
static seq_t *fact_table = NULL;
static size_t fact_cap = 0;   /* number of factorials stored, indices 0 .. fact_cap-1 */

/* Ensure factorials up to index max_n are computed */
static void ensure_factorials(idx_t max_n)
{
    /* need factorials 0..max_n inclusive -> total count = max_n+1 */
    size_t need = (size_t)(max_n + 1);
    if (need <= fact_cap)
        return;  /* already have enough */

    /* resize the table */
    fact_table = (seq_t *)realloc(fact_table, need * sizeof(seq_t));
    if (!fact_table)
        exit(1); /* allocation failure */

    /* initialise if first time */
    size_t start = 1;
    if (fact_cap == 0) {
        fact_table[0] = (seq_t)1;
        fact_cap   = 1;
        start      = 1;
    }

    /* compute missing factorials */
    for (size_t k = start; k < need; ++k)
        fact_table[k] = fact_table[k - 1] * (seq_t)k;

    fact_cap = need;
}

/* Public function required by the harness */
seq_t a(idx_t n)
{
    /* need factorials up to index n+1 (for i=1 case) */
    ensure_factorials(n + 1);

    idx_t limit = n / 2 + 1;          /* floor(n/2)+1 */
    seq_t sum   = 0;

    for (idx_t i = 1; i <= limit; ++i) {
        seq_t term = fact_table[i] * fact_table[n - i + 2];
        sum += term;
    }
    return sum;
}
