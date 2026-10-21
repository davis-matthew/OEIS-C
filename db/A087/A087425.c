/*
 * A087425  a(n) = S(5*n,5)/S(n,5) where S(n,m) = Sum_{k=0..n}
 *          binomial(n,k)*Fibonacci(m*k).
 * https://oeis.org/A087425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = S(5*n,5)/S(n,5) (S(n,m)=sum_{k=0..n}
 *            binom(n,k)*Fibonacci(m*k))
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdlib.h>
#include <stdint.h>

/* Constants for the linear recurrence (see: X^5 - 23105*X^4 + 37360785*X^3 - 4520654985*X^2 + 40931916905*X - 25937424601 = 0) */
static const seq_t C1 = (seq_t)23105ULL;
static const seq_t C2 = (seq_t)37360785ULL;
static const seq_t C3 = (seq_t)4520654985ULL;
static const seq_t C4 = (seq_t)40931916905ULL;
static const seq_t C5 = (seq_t)25937424601ULL;

/* Static storage for memoization */
static seq_t *memo = NULL;
static size_t memo_len = 0;

/* Tables for small n (needed to compute the first 5 terms exactly) */
static unsigned __int128 fib128[126];
static uint64_t binom[26][26];
static int tables_initialized = 0;

/* Helper: initialize tables once */
static void init_tables(void) {
    if (tables_initialized) return;
    tables_initialized = 1;

    /* Fibonacci up to index 125 (5*5*5) using 128‑bit integers */
    fib128[0] = 0;
    fib128[1] = 1;
    for (int i = 2; i <= 125; ++i) fib128[i] = fib128[i-1] + fib128[i-2];

    /* Binomial coefficients up to n = 25 */
    for (int n = 0; n <= 25; ++n) {
        binom[n][0] = binom[n][n] = 1;
        for (int k = 1; k < n; ++k) {
            binom[n][k] = binom[n-1][k-1] + binom[n-1][k];
        }
    }
}

/* Compute S(n,5) using 128‑bit arithmetic (only for n <= 25) */
static unsigned __int128 computeS128(unsigned n) {
    unsigned __int128 sum = 0;
    for (unsigned k = 0; k <= n; ++k) {
        unsigned idx = 5 * k;          /* 5*k <= 125 for n <= 25 */
        unsigned __int128 term = (unsigned __int128)binom[n][k] * fib128[idx];
        sum += term;
    }
    return sum;
}

/* Main function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* undefined for offset 0, return 0 */

    /* Allocate memoization array */
    if (memo_len < (size_t)n) {
        size_t new_len = (size_t)n;
        if (memo == NULL) {
            memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        } else {
            memo = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
        }
        /* Fill new entries */
        for (size_t i = memo_len; i < (size_t)n; ++i) {
            unsigned idx = (unsigned)i;          /* 0‑based */
            if (idx + 1 <= 5) {                  /* first five terms */
                init_tables();                  /* ensure tables */
                unsigned op_n = idx + 1;
                unsigned __int128 s_n   = computeS128(op_n);
                unsigned __int128 s_5n  = computeS128(5 * op_n);
                /* safety: division is exact by definition */
                memo[idx] = (seq_t)(s_5n / s_n);
            } else {                            /* recurrence for n >= 6 */
                seq_t t1 = C1 * memo[idx-1] + C3 * memo[idx-3] + C5 * memo[idx-5];
                seq_t t2 = C2 * memo[idx-2] + C4 * memo[idx-4];
                memo[idx] = t1 - t2;
            }
        }
        memo_len = n;
    }

    return memo[n-1];
}
