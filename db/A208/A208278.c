/*
 * A208278  Row sums of Pascal's triangle mod 10 (A008975).
 * https://oeis.org/A208278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} ( binom(n,k) mod 10 )
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The binomial row is stored modulo 10 in an array of unsigned
   chars – that is always safe regardless of the concrete type
   of seq_t.  The running sums are cached in a seq_t array so
   that subsequent calls do not need to recompute earlier rows. */

static unsigned char *row = NULL;      /* current row modulo 10   */
static size_t current_len = 0;         /* number of elements in row */
static idx_t current_n = (idx_t)-1;    /* last n processed        */

static seq_t *sum_cache = NULL;       /* cached a(n) values      */
static size_t cache_cap = 0;           /* allocated size of sum_cache */

/* Helper to grow an array of seq_t if necessary. */
static void ensure_sum_cache(idx_t n)
{
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(sum_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1); /* out of memory */
        sum_cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Return cached value if we already have it. */
    if (current_n != (idx_t)-1 && n <= current_n)
        return sum_cache[n];

    /* Initialise the first row (n = 0). */
    if (current_n == (idx_t)-1) {
        row = (unsigned char *)malloc(sizeof(unsigned char));
        if (!row) exit(1);
        row[0] = 1;
        current_len = 1;
        current_n = 0;

        sum_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!sum_cache) exit(1);
        sum_cache[0] = (seq_t)1;
        cache_cap = 1;
    }

    /* Extend rows up to the requested n. */
    for (idx_t m = current_n + 1; m <= n; ++m) {
        size_t new_len = (size_t)m + 1;   /* elements in row m */
        unsigned char *tmp = (unsigned char *)realloc(row, new_len * sizeof(unsigned char));
        if (!tmp) exit(1);
        row = tmp;

        row[new_len - 1] = 1;             /* outer element */

        /* Update interior elements from right to left. */
        for (idx_t i = new_len - 2; i >= 1; --i)
            row[i] = (unsigned char)((row[i - 1] + row[i]) % 10);
        /* row[0] remains 1. */

        /* Compute the sum of this row. */
        seq_t sum = 0;
        for (size_t i = 0; i < new_len; ++i)
            sum += (seq_t)row[i];

        /* Keep cache large enough. */
        ensure_sum_cache(m);
        sum_cache[m] = sum;
        current_n = m;
    }

    return sum_cache[n];
}
