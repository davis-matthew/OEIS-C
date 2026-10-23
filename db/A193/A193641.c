/*
 * A193641  Number of arrays of -1..1 integers x(1..n) with every x(i) in a
 *          subsequence of length 1 or 2 with sum zero.
 * https://oeis.org/A193641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-3) with initial values a(0)=1, a(1)=1,
 *            a(2)=3
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#  include <new>
#endif
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* static cache */
static seq_t *memo   = 0;    /* pointer to cached values */
static idx_t memo_len = 0;   /* number of elements currently stored */
static idx_t memo_cap = 0;   /* allocated capacity */

/* allocate or resize the cache to hold at least 'cap' elements */
static void ensure_capacity(idx_t cap)
{
    if (cap <= memo_cap) return;

    /* allocate new storage */
#ifdef __cplusplus
    seq_t *new_mem = new seq_t[cap];
#else
    seq_t *new_mem = (seq_t *)malloc(cap * sizeof(seq_t));
#endif
    if (!new_mem) {
        /* allocation failure – abort as the original implementation did */
        exit(1);
    }

    /* copy existing elements */
    for (idx_t i = 0; i < memo_len; ++i) {
        new_mem[i] = memo[i];
    }

    /* free old storage */
#ifdef __cplusplus
    delete [] memo;
#else
    free(memo);
#endif
    memo = new_mem;
    memo_cap = cap;
}

seq_t a(idx_t n)
{
    /* make sure the cache can hold n+1 elements (index 0 .. n) */
    if (memo_cap == 0) {
        /* initial capacity: at least 64, but not less than n+1 */
        idx_t init_cap = 64;
        if (n + 1 > init_cap) init_cap = n + 1;
        ensure_capacity(init_cap);
    }
    if (n + 1 > memo_cap) {
        /* double the capacity until it is large enough */
        idx_t new_cap = memo_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        ensure_capacity(new_cap);
    }

    /* fill in missing values up to n */
    for (idx_t i = memo_len; i <= n; ++i) {
        if (i == 0)        memo[i] = 1;
        else if (i == 1)   memo[i] = 1;
        else if (i == 2)   memo[i] = 3;
        else               memo[i] = 2 * memo[i-1] + memo[i-3];
    }
    memo_len = n + 1;

    return memo[n];
}
