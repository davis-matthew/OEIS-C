/*
 * A108188  a(n) = (n-1)*(a(n-1)+a(n-2)+a(n-3)).
 * https://oeis.org/A108188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*(a(n-1)+a(n-2)+a(n-3)); Initial values: a(1)=0,
 *            a(2)=1, a(3)=1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for memoization */
static seq_t *memo = NULL;     /* array holding computed terms */
static idx_t memo_len = 0;      /* highest index that has been computed */
static idx_t memo_cap = 0;      /* current capacity of memo array */

/* Ensure that memo array holds terms up to the given index n.
   Allocate or grow the array as needed and compute any missing terms. */
static void ensure(idx_t n) {
    if (n <= memo_len) return;                     /* already computed */

    /* grow capacity if necessary */
    if (n + 1 > memo_cap) {
        idx_t newcap = memo_cap;
        if (newcap == 0) newcap = 4;
        while (newcap <= n) newcap <<= 1;          /* double until enough */

        seq_t *newmem = (seq_t *)realloc(memo, newcap * sizeof(seq_t));
        if (!newmem) abort();                     /* allocation failure */

        memo = newmem;
        memo_cap = newcap;
        /* initialize new slots to zero for safety */
        for (idx_t i = memo_len + 1; i < memo_cap; ++i) memo[i] = (seq_t)0;
    }

    /* Compute terms from memo_len+1 up to n */
    for (idx_t i = memo_len + 1; i <= n; ++i) {
        if (i == 1)
            memo[i] = (seq_t)0;
        else if (i == 2 || i == 3)
            memo[i] = (seq_t)1;
        else {
            seq_t sum = memo[i - 1] + memo[i - 2] + memo[i - 3];
            memo[i] = ((seq_t)(i - 1)) * sum;
        }
    }
    memo_len = n;
}

/* Public sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined for offset 1, but safe */
    ensure(n);
    return memo[n];
}
