/*
 * A192564  a(n) = Sum_{k=0..n} abs(Stirling1(n,k))*Stirling2(n,k)*(k!)^2.
 * https://oeis.org/A192564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} |Stirling1(n,k)| * Stirling2(n,k) * (k!)^2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The unsigned Stirling numbers of the first kind (c) */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t *C_prev = NULL, *C_curr = NULL;
    static seq_t *S_prev = NULL, *S_curr = NULL;
    static seq_t *fact   = NULL;
    static seq_t *memo   = NULL;
    static size_t capacity = 0;
    static idx_t computed = (idx_t)-1;   /* no values computed yet */

    size_t N = (size_t)n;   /* use size_t for loop indices */

    /* initialise on first call */
    if (computed == (idx_t)-1) {
        capacity = 1;
        C_prev = (seq_t*)malloc(capacity * sizeof(seq_t));
        C_curr = (seq_t*)malloc(capacity * sizeof(seq_t));
        S_prev = (seq_t*)malloc(capacity * sizeof(seq_t));
        S_curr = (seq_t*)malloc(capacity * sizeof(seq_t));
        fact   = (seq_t*)malloc(capacity * sizeof(seq_t));
        memo   = (seq_t*)malloc(capacity * sizeof(seq_t));
        for (size_t i = 0; i < capacity; ++i)
            C_prev[i] = C_curr[i] = S_prev[i] = S_curr[i] = fact[i] = memo[i] = 0;
        C_prev[0] = 1;   /* c(0,0) = 1 */
        S_prev[0] = 1;   /* S(0,0) = 1 */
        fact[0]   = 1;   /* 0! = 1 */
        memo[0]   = 1;   /* a(0) = 1 */
        computed  = 0;
    }

    /* grow arrays if necessary */
    if (N >= capacity) {
        size_t new_cap = N + 1;
        C_prev = (seq_t*)realloc(C_prev, new_cap * sizeof(seq_t));
        C_curr = (seq_t*)realloc(C_curr, new_cap * sizeof(seq_t));
        S_prev = (seq_t*)realloc(S_prev, new_cap * sizeof(seq_t));
        S_curr = (seq_t*)realloc(S_curr, new_cap * sizeof(seq_t));
        fact   = (seq_t*)realloc(fact,   new_cap * sizeof(seq_t));
        memo   = (seq_t*)realloc(memo,   new_cap * sizeof(seq_t));
        /* initialise new slots to zero */
        for (size_t i = capacity; i < new_cap; ++i) {
            C_prev[i] = C_curr[i] = S_prev[i] = S_curr[i] = fact[i] = memo[i] = 0;
        }
        capacity = new_cap;
    }

    /* compute values up to N */
    for (size_t i = computed + 1; i <= N; ++i) {
        /* factorial */
        fact[i] = fact[i-1] * (seq_t)i;

        /* unsigned Stirling numbers of the first kind (c) */
        C_curr[0] = 0;
        /* unsigned Stirling numbers of the second kind (S) */
        S_curr[0] = 0;
        for (size_t k = 1; k <= i; ++k) {
            /* c(i,k) = c(i-1,k-1) + (i-1)*c(i-1,k) */
            C_curr[k] = C_prev[k-1] + (seq_t)(i-1) * C_prev[k];
            /* S(i,k) = k*S(i-1,k) + S(i-1,k-1) */
            S_curr[k] = (seq_t)k * S_prev[k] + S_prev[k-1];
        }

        /* compute a(i) */
        seq_t sum = 0;
        for (size_t k = 0; k <= i; ++k) {
            seq_t term = C_curr[k];
            term = term * S_curr[k];
            term = term * fact[k];
            term = term * fact[k];
            sum  = sum + term;
        }
        memo[i] = sum;

        /* prepare for next iteration */
        seq_t *tmp = C_prev; C_prev = C_curr; C_curr = tmp;
        tmp = S_prev;       S_prev = S_curr;      S_curr = tmp;
    }

    computed = N;
    return memo[N];
}
