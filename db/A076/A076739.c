/*
 * A076739  Number of compositions of n into Fibonacci numbers (1 counted as
 *          single Fibonacci number).
 * https://oeis.org/A076739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = sum_{F = Fibonacci number <= n} a(n - F) (OEIS
 *            A076739)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 35, u64 n <= 70, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* external integer types (supplied by the build) */
extern seq_t a(idx_t n);

static idx_t fib_limit = 0;          /* largest Fibonacci number generated */
static seq_t *fib_arr = NULL;        /* array of unique Fibonacci numbers 1,2,3,... */
static idx_t fib_cnt = 0;            /* number of Fibonacci numbers stored */

/* dynamic array for the DP values a(0)..a(n) */
static seq_t *dp_arr = NULL;
static idx_t dp_size = 0;           /* current allocated size */

/* Allocate / resize dp_arr up to index n (inclusive) and zero the new entries */
static void ensure_dp(idx_t n)
{
    if (dp_size <= n) {
        idx_t old = dp_size;
        idx_t new_size = n + 1;
        dp_arr = (seq_t *)realloc(dp_arr, new_size * sizeof(seq_t));
        if (!dp_arr) exit(1);      /* memory failure aborts */
        for (idx_t i = old; i <= n; ++i)
            dp_arr[i] = 0;
        dp_size = new_size;
    }
}

/* Ensure fib_arr contains all unique Fibonacci numbers <= n */
static void ensure_fib(idx_t n)
{
    if (fib_limit >= n) return;  /* already up to date */

    /* If this is the first time, start from the beginning of the Fibonacci sequence */
    seq_t a_val = 1, b_val = 2;   /* 1, 2 are the first two unique Fibonacci numbers */

    if (fib_cnt == 0) {
        fib_arr = (seq_t *)malloc(4 * sizeof(seq_t));  /* small initial capacity */
        if (!fib_arr) exit(1);
        fib_arr[0] = (seq_t)1;
        fib_arr[1] = (seq_t)2;
        fib_cnt = 2;
    } else {
        /* Continue from the last two unique Fibonacci numbers stored */
        a_val = fib_arr[fib_cnt - 2];
        b_val = fib_arr[fib_cnt - 1];
    }

    while (1) {
        seq_t next = a_val + b_val;
        if (next > (seq_t)n) break;
        if (fib_cnt >= 4) {   /* simple dynamic resize */
            fib_arr = (seq_t *)realloc(fib_arr, (fib_cnt + 4) * sizeof(seq_t));
            if (!fib_arr) exit(1);
        }
        fib_arr[fib_cnt++] = next;
        a_val = b_val;
        b_val = next;
    }
    fib_limit = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Preserve old size to compute only new entries */
    idx_t prev_size = dp_size;

    /* Ensure storage for DP values up to n */
    ensure_dp(n);

    /* Base case */
    if (dp_arr[0] == 0) dp_arr[0] = 1;

    /* Ensure Fibonacci numbers up to n are ready */
    ensure_fib(n);

    /* Compute DP values from prev_size (or 0) up to n */
    for (idx_t i = prev_size; i <= n; ++i) {
        if (i == 0) continue;  /* already set */
        seq_t sum = 0;
        for (idx_t j = 0; j < fib_cnt; ++j) {
            seq_t f = fib_arr[j];
            if (f > (seq_t)i) break;
            sum += dp_arr[i - (idx_t)f];
        }
        dp_arr[i] = sum;
    }
    return dp_arr[n];
}
