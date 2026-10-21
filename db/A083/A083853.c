/*
 * A083853  Smallest nonnegative integer that can be written as a sum of
 *          Fibonacci numbers in n ways, counting 1 twice as Fibonacci number.
 * https://oeis.org/A083853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest non‑negative integer that can be * written
 *            as a sum of Fibonacci numbers in exactly n distinct ways, *
 *            treating the two occurrences of 1 in the Fibonacci sequence as *
 *            distinct coins. There is no closed formula; we compute it by *
 *            counting subsets of the Fibonacci coins
 * Range:     u8 n <= 22, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
#define MAX_N 20000ULL          /* maximum n we will precompute */
#define MAX_M 200000ULL         /* maximum integer we examine */
#define LIMIT ((unsigned long long)(MAX_N + 1ULL))   /* cap for counts */

/* --------------------------------------------------------------------- */
/* static state shared by all calls to a() */
static seq_t *dp_global  = NULL;      /* dp[j] = number of subsets giving sum j */
static seq_t *ans_global = NULL;      /* ans_global[n] = smallest m with n ways */
static unsigned char *seen   = NULL;  /* 1 if ans_global[n] found */
static bool table_built = false;

/* --------------------------------------------------------------------- */
static void build_tables(void)
{
    size_t sz = (size_t)MAX_M + 1ULL;
    dp_global = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp_global) abort();
    dp_global[0] = 1ULL;      /* one way to make 0: empty subset */

    /* Generate Fibonacci numbers (with two 1's) up to MAX_M */
    unsigned long long fibs[64];
    size_t fib_cnt = 0;
    fibs[fib_cnt++] = 1ULL;            /* first 1 */
    fibs[fib_cnt++] = 1ULL;            /* second 1 */
    while (true) {
        unsigned long long next = fibs[fib_cnt-1] + fibs[fib_cnt-2];
        if (next > MAX_M) break;
        fibs[fib_cnt++] = next;
    }

    /* 0/1 knapsack: count subsets (each coin used at most once) */
    for (size_t fi = 0; fi < fib_cnt; ++fi) {
        unsigned long long coin = fibs[fi];
        size_t coin_u = (size_t)coin;
        for (size_t j = sz - 1; j >= coin_u; --j) {
            unsigned long long prev = (unsigned long long)dp_global[j];
            unsigned long long add  = (unsigned long long)dp_global[j - coin_u];
            unsigned long long sum = prev + add;
            if (sum > LIMIT) sum = LIMIT;
            dp_global[j] = (seq_t)sum;
            if (j == coin_u) break; /* prevent wrap‑around */
        }
    }

    /* Allocate answer tables */
    ans_global = (seq_t *)calloc((size_t)MAX_N + 1ULL, sizeof(seq_t));
    seen      = (unsigned char *)calloc((size_t)MAX_N + 1ULL, sizeof(unsigned char));
    if (!ans_global || !seen) abort();

    /* Record first occurrence of each count up to MAX_N */
    size_t found = 0;
    for (size_t m = 0ULL; m <= MAX_M; ++m) {
        seq_t cnt = dp_global[m];
        if ((unsigned long long)cnt <= MAX_N) {
            size_t c = (size_t)cnt;
            if (!seen[c]) {
                ans_global[c] = (seq_t)m;
                seen[c] = 1;
                ++found;
                if (found > MAX_N) break;
            }
        }
    }
}

/* --------------------------------------------------------------------- */
/* Public entry function */
seq_t a(idx_t n)
{
    if (!table_built) {
        build_tables();
        table_built = true;
    }
    if (n == 0) return 0ULL;     /* not used, but safe */
    if ((size_t)n <= MAX_N) {
        return ans_global[(size_t)n];
    }
    return 0ULL; /* out of precomputed range */
}
