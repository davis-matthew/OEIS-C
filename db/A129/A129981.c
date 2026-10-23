/*
 * A129981  Sum of n!!, with n>=0.
 * https://oeis.org/A129981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = sum_{k=0..n} k!!   (partial sums of double factorials)
 * Description: The function memoizes previously computed values so that 
 * any subsequent call with the same or smaller argument is answered in O(1).
 * Complexity for the first call up to n is O(n) – each double factorial
 * is computed from the two earlier ones and its cumulative sum is updated.
 */

#include <stdlib.h>

static seq_t *df   = NULL;   /* df[i] = i!!          */
static seq_t *sums = NULL;   /* sums[i] = a(i)       */
static idx_t  max_n = (idx_t)-1;   /* highest n computed so far */
static size_t alloc_cap = 0;   /* current capacity of arrays */

/* Ensure the tables have at least new_cap entries. */
static void ensure_capacity(idx_t new_cap) {
    if ((size_t)new_cap <= alloc_cap) return;

    if (alloc_cap == 0) {
        df   = (seq_t*)malloc(new_cap * sizeof(seq_t));
        sums = (seq_t*)malloc(new_cap * sizeof(seq_t));
    } else {
        df   = (seq_t*)realloc(df,   new_cap * sizeof(seq_t));
        sums = (seq_t*)realloc(sums, new_cap * sizeof(seq_t));
    }
    if (!df || !sums) {
        /* Allocation failed – terminate, as the sequence cannot continue. */
        exit(EXIT_FAILURE);
    }
    alloc_cap = (size_t)new_cap;
}

/* Compute a(n) = sum_{k=0..n} k!!. */
seq_t a(idx_t n) {
    /* First call: initialise for n = 0. */
    if (max_n == (idx_t)-1) {
        ensure_capacity(1);
        df[0]   = 1;   /* 0!! = 1 */
        sums[0] = 1;   /* a(0) = 1 */
        max_n   = 0;
    }

    if (n <= max_n) {
        return sums[n];
    }

    /* Extend precomputation up to n. */
    ensure_capacity(n + 1);

    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i <= 1) {
            df[i] = 1;                  /* 0!! = 1, 1!! = 1 */
        } else {
            /* i!! = i * (i-2)!! */
            df[i] = (seq_t)i * df[i - 2];
        }
        sums[i] = sums[i - 1] + df[i];
    }

    max_n = n;
    return sums[n];
}
