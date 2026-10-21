/*
 * A218396  Number of compositions of n into distinct (nonzero) Fibonacci
 *          numbers.
 * https://oeis.org/A218396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{S subset of distinct Fibonacci numbers} (|S|)! for
 *            subsets S summing to n
 * Range:     u8 n <= 31, u16 n <= 141, u32 n <= 984, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* helper: factorial table up to 20!  (20 distinct Fibonacci numbers ≤ 10000) */
static seq_t fact[21];
static bool fact_inited = false;
static void init_factorial(void) {
    if (fact_inited) return;
    fact[0] = (seq_t)1;
    for (int i = 1; i <= 20; ++i) {
        fact[i] = fact[i-1] * (seq_t)i;
    }
    fact_inited = true;
}

/* Fibonacci numbers ≤ 10000 (distinct values): 1,2,3,5,8,... */
static uint64_t fibs[32];
static size_t fib_count = 0;
static bool fib_inited = false;
static void init_fibs(void) {
    if (fib_inited) return;
    uint64_t a = 1, b = 2;
    while (a <= 10000) {
        fibs[fib_count++] = a;
        uint64_t c = a + b;
        a = b;
        b = c;
    }
    fib_inited = true;
}

seq_t a(idx_t n) {
    /* ensure helpers are ready */
    init_fibs();
    init_factorial();

    size_t N = (size_t)n;
    size_t rows = fib_count + 1;
    size_t cols = N + 1;

    /* dp array: dp[k][s] = number of subsets of size k summing to s */
    seq_t *dp = (seq_t *)calloc(rows * cols, sizeof(seq_t));
    if (!dp) return 0;   /* allocation failure */

    #define IDX(k,s) ((k)*(cols)+(s))

    dp[IDX(0,0)] = (seq_t)1;

    for (size_t fi = 0; fi < fib_count; ++fi) {
        size_t f = fibs[fi];
        int maxk = (int)fi + 1;
        if (f > N) continue;   /* this Fibonacci number can never be used */

        for (int k = maxk; k >= 1; --k) {
            for (size_t s = N; ; --s) {
                dp[IDX(k,s)] = dp[IDX(k,s)] + dp[IDX(k-1,s-f)];
                if (s == f) break;
            }
        }
    }

    seq_t result = (seq_t)0;
    for (size_t k = 0; k <= fib_count; ++k) {
        seq_t cnt = dp[IDX(k,N)];
        if (cnt) result = result + cnt * fact[k];
    }

    free(dp);
    return result;
}
