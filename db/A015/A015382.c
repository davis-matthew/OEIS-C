/*
 * A015382  Gaussian binomial coefficient [ n,9 ] for q=-10.
 * https://oeis.org/A015382
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Product_{i=1..9} ((-10)^(n-i+1)-1)/((-10)^i-1)
 * Range:     i8 n <= 9, i16 n <= 9, i32 n <= 10, i64 n <= 11, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                      Gaussian binomial coefficient [n,9] for q = -10, offset 9 */

#include <stdlib.h>

static seq_t *pow_arr  = NULL;   /* (-10)^e for e = 0..max_exp */
static size_t pow_cap  = 0;

static seq_t *cache    = NULL;   /* cached a(n) for n >= 0 */
static size_t cache_cap = 0;

static seq_t prev_row[10];       /* previous row of the q-binomial DP */
static idx_t curr_max_n = 0;     /* highest n that has been computed */

/* Ensure pow_arr has entries up to exponent `max_exp`. */
static void ensure_pow(idx_t max_exp) {
    if ((size_t)max_exp + 1 <= pow_cap) return;

    size_t new_cap = (size_t)max_exp + 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) abort();

    /* copy existing entries */
    for (size_t i = 0; i < pow_cap; ++i) new_arr[i] = pow_arr[i];

    seq_t base = (seq_t)(-10);
    if (pow_cap == 0) {
        new_arr[0] = (seq_t)1;
        pow_cap = 1;
    }

    /* compute new entries */
    for (size_t i = pow_cap; i < new_cap; ++i) {
        new_arr[i] = new_arr[i - 1] * base;
    }

    free(pow_arr);
    pow_arr = new_arr;
    pow_cap = new_cap;
}

/* Ensure cache can store indices up to `max_n`. */
static void ensure_cache(idx_t max_n) {
    if ((size_t)max_n + 1 <= cache_cap) return;

    size_t new_cap = (size_t)max_n + 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) abort();

    for (size_t i = 0; i < cache_cap; ++i) new_arr[i] = cache[i];

    free(cache);
    cache = new_arr;
    cache_cap = new_cap;
}

/* Compute all values up to n (inclusive). */
static void extend_to(idx_t n) {
    if (n < 9) return;
    if (curr_max_n >= n) return;

    /* Prepare auxiliary arrays. */
    ensure_pow(n - 1);          /* need powers up to exponent n-1 */
    ensure_cache(n);            /* need cache up to index n */

    /* Initialize first row if fresh. */
    if (curr_max_n == 0) {
        for (int i = 0; i <= 9; ++i) prev_row[i] = 0;
        prev_row[0] = (seq_t)1;
    }

    /* DP over rows. */
    for (idx_t m = curr_max_n + 1; m <= n; ++m) {
        seq_t cur[10];
        cur[0] = (seq_t)1;
        int limit = m < 9 ? (int)m : 9;
        for (int i = 1; i <= limit; ++i) {
            seq_t temp = pow_arr[(size_t)(m - i)] * prev_row[i - 1];
            cur[i] = prev_row[i] + temp;
        }
        for (int i = limit + 1; i <= 9; ++i) cur[i] = (seq_t)0;

        if (m >= 9) cache[(size_t)m] = cur[9];

        /* Move cur to prev_row for next iteration. */
        for (int i = 0; i <= 9; ++i) prev_row[i] = cur[i];
    }

    curr_max_n = n;
}

seq_t a(idx_t n) {
    if (n < 9) return (seq_t)0;   /* behavior below offset is irrelevant */
    extend_to(n);
    return cache[(size_t)n];
}
