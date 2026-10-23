/*
 * A103645  G.f.: (108*x^2+27*x+1)/(1+2*x-6*x^2-27*x^3).
 * https://oeis.org/A103645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = -2*a(n-1) + 6*a(n-2) + 27*a(n-3), with initial values
 *            a(0)=1, a(1)=25, a(2)=64
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)25;
    if (n == 2) return (seq_t)64;

    static seq_t *memo = NULL;
    static idx_t memo_size   = 0;  /* Number of terms stored (up to memo_size-1) */
    static idx_t memo_capacity = 0;/* Allocated size of memo */

    /* Allocate or grow storage if needed */
    if (memo == NULL) {
        idx_t cap = n + 1;
        if (cap < 3) cap = 3;
        memo = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!memo) exit(1); /* allocation failed */
        memo_capacity = cap;
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)25;
        memo[2] = (seq_t)64;
        memo_size = 3;
    } else if (memo_capacity <= n) {
        idx_t cap = n + 1;
        seq_t *newptr = (seq_t *)realloc(memo, cap * sizeof(seq_t));
        if (!newptr) exit(1);
        memo = newptr;
        memo_capacity = cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = memo_size; i <= n; ++i) {
        seq_t temp = (seq_t)27 * memo[i-3] + (seq_t)6 * memo[i-2];
        memo[i] = temp - (seq_t)2 * memo[i-1];
    }

    memo_size = n + 1;
    return memo[n];
}
