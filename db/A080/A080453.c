/*
 * A080453  a(1) = 9, a(n) = smallest (nontrivial) multiple of a(n-1)
 *          containing n digits, a(n) not equal to 10*a(n-1).
 * https://oeis.org/A080453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A080453:
   a(1) = 9,
   for n > 1, a(n) = the smallest non‑trivial multiple of a(n‑1) that
   contains exactly n decimal digits and is not equal to 10·a(n‑1). */
/* The implementation memoizes previously computed terms to allow
   arbitrary calling order and to keep the run time linear in n. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#  include <new>
#endif

/* memoization storage ----------------------------------------------------- */
static seq_t *memo      = NULL;  /* array of previously computed terms      */
static size_t memo_size = 0;     /* number of elements actually stored      */
static size_t memo_cap  = 0;     /* current allocated capacity              */
static seq_t pow10_cur  = 0;     /* 10^(current_index‑1) (starts at 10^0)   */
static int   initialized = 0;    /* flag: has the table been set up?        */

/* Ensure that the memo array can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (memo_cap >= need) return;

    size_t new_cap = memo_cap ? memo_cap * 2 : 8;
    if (new_cap < need) new_cap = need;

#ifdef __cplusplus
    seq_t *new_arr = new seq_t[new_cap];
    if (memo) {
        for (size_t i = 0; i < memo_size; ++i) new_arr[i] = memo[i];
        delete[] memo;
    }
    memo = new_arr;
#else
    seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    if (memo) {
        for (size_t i = 0; i < memo_size; ++i) new_arr[i] = memo[i];
        free(memo);
    }
    memo = new_arr;
#endif
    memo_cap = new_cap;
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialize on the first call. */
    if (!initialized) {
#ifdef __cplusplus
        memo = new seq_t[8];
#else
        memo = (seq_t*)malloc(8 * sizeof(seq_t));
        if (!memo) abort();
#endif
        memo[0]      = (seq_t)9;  /* a(1) */
        memo_size    = 1;
        pow10_cur    = (seq_t)1;  /* 10^0 */
        initialized  = 1;
    }

    /* Compute terms up to the requested index n. */
    while (memo_size < (size_t)n) {
        /* pow10_cur becomes 10^(current_index-1) */
        pow10_cur *= (seq_t)10;

        seq_t prev = memo[memo_size - 1];

        /* k = ceil(10^(n-1) / prev) */
        seq_t k = (pow10_cur + prev - (seq_t)1) / prev;

        /* Skip the forbidden multiple 10 * a(n-1). */
        if (k == (seq_t)10) k = (seq_t)11;

        seq_t val = prev * k;

        ensure_capacity(memo_size + 1);
        memo[memo_size++] = val;
    }

    return memo[n - 1];
}
