/*
 * A175108  a(n) = number of partitions of d(n) into d(k)'s, where the k's are
 *          each <= n and distinct, but the d(k)'s need not be distinct. Here
 *          d(m) = the number of divisors of m.
 * https://oeis.org/A175108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^d(n) in Product_{k=1..n} (1 + x^d(k))
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*       where d(m) is the number of divisors of m. */
/* Implementation: incremental subset–sum dynamic programming. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Small helper: divisor count using simple trial division.  This is
   adequate for n up to at least 10^6 (and well below the time limits). */
static uint32_t divisor_count(idx_t x)
{
    uint32_t n = (uint32_t)x;          /* n is small (≤ 10⁶) in practice */
    uint32_t res = 1;
    uint32_t temp = n;

    for (uint32_t p = 2; (uint64_t)p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint32_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            res *= (cnt + 1);
        }
    }
    if (temp > 1) {
        res *= 2;      /* remaining prime factor */
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Main sequence function */

seq_t a(idx_t n)
{
    /* static state for out‑of‑order calls */
    static bool init_done = false;

    /* DP array for subset sums: dp[s] = number of subsets with sum s */
    static seq_t *dp = NULL;
    static size_t dp_cap = 0;         /* allocated size of dp */
    static size_t max_sum = 0;        /* current maximum sum in dp */

    /* Divisor counts already computed */
    static uint32_t *divcnt = NULL;
    static size_t divcnt_cap = 0;

    /* Answers for n = 0..current_n */
    static seq_t *answers = NULL;
    static size_t answers_cap = 0;

    /* Last n processed by the algorithm */
    static idx_t last_processed_n = 0;

    /* ------------------------------------------------------------------ */
    /* Initialization */
    if (!init_done) {
        init_done = true;

        /* dp[0] = 1 (empty subset) */
        dp_cap = 1;
        dp = (seq_t *)malloc(dp_cap * sizeof(seq_t));
        if (!dp) return 0;
        dp[0] = 1;

        /* capacities */
        divcnt_cap = 64;
        divcnt = (uint32_t *)malloc(divcnt_cap * sizeof(uint32_t));
        if (!divcnt) return 0;

        answers_cap = 64;
        answers = (seq_t *)malloc(answers_cap * sizeof(seq_t));
        if (!answers) return 0;

        /* answers[0] unused, leave it zero */
    }

    /* ------------------------------------------------------------------ */
    /* Handle n < 1 gracefully */
    if (n < 1) return 0;

    /* ------------------------------------------------------------------ */
    /* Expand state if we need to process a larger n */
    if (n > last_processed_n) {
        /* Ensure capacity of divcnt and answers arrays */
        size_t needed = (size_t)n + 1;
        if (needed > divcnt_cap) {
            size_t new_cap = divcnt_cap ? divcnt_cap * 2 : 64;
            while (new_cap < needed) new_cap <<= 1;
            uint32_t *tmp = (uint32_t *)realloc(divcnt, new_cap * sizeof(uint32_t));
            if (!tmp) return 0;
            divcnt = tmp;
            divcnt_cap = new_cap;
        }

        if (needed > answers_cap) {
            size_t new_cap = answers_cap ? answers_cap * 2 : 64;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(answers, new_cap * sizeof(seq_t));
            if (!tmp) return 0;
            answers = tmp;
            answers_cap = new_cap;
        }

        /* Process new indices k = last_processed_n+1 .. n */
        for (idx_t k = last_processed_n + 1; k <= n; ++k) {
            uint32_t weight = divisor_count(k);
            divcnt[(size_t)k] = weight;

            /* Expand dp array to accommodate new maximum sum */
            size_t new_max_sum = max_sum + weight;   /* inclusive upper bound */
            if (new_max_sum + 1 > dp_cap) {
                size_t new_cap = dp_cap ? dp_cap * 2 : 64;
                while (new_cap <= new_max_sum) new_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
                if (!tmp) return 0;
                for (size_t i = dp_cap; i < new_cap; ++i) tmp[i] = 0;
                dp = tmp;
                dp_cap = new_cap;
            }

            /* Subset‑sum DP: iterate backwards to avoid reuse of element k */
            for (size_t s = new_max_sum; s >= (size_t)weight; --s) {
                dp[s] += dp[s - (size_t)weight];
                if (s == (size_t)weight) break;   /* avoid wrap‑around of size_t */
            }

            max_sum = new_max_sum;
            answers[(size_t)k] = dp[(size_t)weight];
        }

        last_processed_n = n;
    }

    return answers[(size_t)n];
}
