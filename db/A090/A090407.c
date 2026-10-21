/*
 * A090407  a(n) = Sum_{k = 0..n} C(4*n + 1, 4*k).
 * https://oeis.org/A090407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/2) * 4^n * (4^n + (-1)^n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=6, a(n)=12*a(n-1)+64*a(n-2) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* memoization to allow arbitrary call order */
    static seq_t *memo = NULL;   /* array of previously computed terms */
    static idx_t memo_len = 0;   /* number of elements stored in memo */

    /* Ensure memo has space for indices up to n */
    if (memo_len <= 1) {
        /* First time: allocate space for at least 2 elements */
        memo = (seq_t *)realloc(memo, sizeof(seq_t) * (n + 1));
        if (memo == NULL) abort();   /* allocation failure */
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)6;
        memo_len = 2;
    }
    else if (n + 1 > memo_len) {
        /* Expand array if needed */
        memo = (seq_t *)realloc(memo, sizeof(seq_t) * (n + 1));
        if (memo == NULL) abort();
    }

    /* Compute any missing values up to n */
    while (memo_len <= n) {
        memo[memo_len] = (seq_t)12 * memo[memo_len - 1] + (seq_t)64 * memo[memo_len - 2];
        ++memo_len;
    }

    return memo[n];
}
