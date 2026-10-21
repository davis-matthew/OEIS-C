/*
 * A239002  Number of partitions of n into distinct parts all of which are
 *          Fibonacci numbers greater than 1.
 * https://oeis.org/A239002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = number of partitions of n into distinct parts that are
 *            Fibonacci numbers > 1. Implemented by a standard
 *            unbounded‑knapsack DP with each Fibonacci coin used at most once
 * Range:     u8 n <= 9520, u16 n <= 9520, u32 n <= 9520, u64 n <= 9520,
 *            u128 n <= 9520, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static state for memoisation */
static seq_t *dp      = NULL; /* dp[0..max_n] */
static size_t dp_cap  = 0;    /* allocated capacity of dp */

static idx_t *fib_arr = NULL; /* Fibonacci numbers > 1 */
static size_t fib_cap = 0;    /* allocated capacity of fib_arr */
static size_t fib_cnt = 0;    /* current number of fib numbers stored */

/* grow an array of idx_t to hold at least new_cap elements. */
static void grow_idx_array(idx_t **arr, size_t *cap, size_t new_cap)
{
    if (*cap >= new_cap) return;
    size_t new_size = *cap == 0 ? 8 : (*cap << 1);
    if (new_size < new_cap) new_size = new_cap;
    idx_t *tmp = (idx_t *)realloc(*arr, new_size * sizeof(idx_t));
    if (tmp == NULL) exit(1);
    *arr = tmp;
    *cap = new_size;
}

/* grow the dp array to at least new_cap elements. */
static void grow_dp(size_t new_cap)
{
    if (dp_cap >= new_cap) return;
    size_t new_size = dp_cap == 0 ? 8 : (dp_cap << 1);
    if (new_size < new_cap) new_size = new_cap;
    seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
    if (tmp == NULL) exit(1);
    dp = tmp;
    dp_cap = new_size;
}

/* compute all Fibonacci numbers > 1 up to n, storing them in fib_arr */
static void generate_fib(idx_t n)
{
    /* initialise first two */
    if (fib_cnt == 0) {
        grow_idx_array(&fib_arr, &fib_cap, fib_cnt + 2);
        fib_arr[0] = (idx_t)2;
        fib_arr[1] = (idx_t)3;
        fib_cnt  = 2;
    }

    /* extend list as needed */
    while (fib_cnt >= 2) {
        idx_t next = fib_arr[fib_cnt - 1] + fib_arr[fib_cnt - 2];
        if (next > n) break;
        grow_idx_array(&fib_arr, &fib_cap, fib_cnt + 1);
        fib_arr[fib_cnt++] = next;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* ensure dp big enough */
    grow_dp((size_t)n + 1);

    /* regenerate Fibonacci list up to n */
    generate_fib(n);

    /* initialise dp array up to n */
    dp[0] = (seq_t)1;
    for (size_t i = 1; i <= (size_t)n; ++i) dp[i] = (seq_t)0;

    /* DP: iterate over each Fibonacci coin once */
    for (size_t fi = 0; fi < fib_cnt; ++fi) {
        idx_t f = fib_arr[fi];
        if (f > n) break;
        for (idx_t j = n; j >= f; --j) {
            dp[j] += dp[j - f];
        }
    }

    return dp[n];
}
