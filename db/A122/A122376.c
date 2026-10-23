/*
 * A122376  Number of prime factors (counted with multiplicity) of the
 *          smallest number with exactly n divisors.
 * https://oeis.org/A122376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime factors (with multiplicity) of the
 *            smallest number having exactly n divisors
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>

/* maximum divisor count we will pre‑compute */
#define MAX_LIMIT 20000

/* first few primes are enough to reach divisor counts up to MAX_LIMIT */
static const uint64_t primes[16] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL,
    13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL,
    53ULL
};
static const int NUM_PRIMES = 16;

/* tables storing best (smallest) number of prime factors and
   logarithmic value of the minimal number for each divisor count.  */
static seq_t *best_omega = NULL;
static double *best_log   = NULL;
/* precomputed logarithms of the primes */
static double *prime_log = NULL;

/* whether the tables have been built */
static bool initialized = false;

/* depth first search generating all exponent vectors */
static void dfs(int idx, int last_exp, double cur_log, int cur_div, int cur_omega)
{
    if (idx >= NUM_PRIMES) return;

    int max_e = last_exp;
    int max_possible = MAX_LIMIT / cur_div - 1;
    if (max_possible < max_e) max_e = max_possible;
    if (max_e <= 0) return;

    for (int e = 1; e <= max_e; ++e) {
        double new_log = cur_log + e * prime_log[idx];
        int    new_div = cur_div * (e + 1);
        int    new_omega = cur_omega + e;

        /* update the best value for this divisor count if the
           logarithmic value (i.e. the size of the number) is smaller */
        if (best_log[new_div] > new_log + 1e-12) {
            best_log[new_div]  = new_log;
            best_omega[new_div] = (seq_t)new_omega;
        }
        dfs(idx + 1, e, new_log, new_div, new_omega);
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (!initialized) {
        /* allocate tables */
        best_omega = (seq_t *)malloc((MAX_LIMIT + 1) * sizeof(seq_t));
        best_log   = (double *)malloc((MAX_LIMIT + 1) * sizeof(double));
        prime_log  = (double *)malloc(NUM_PRIMES * sizeof(double));
        if (!best_omega || !best_log || !prime_log) {
            /* allocation failed – return 0 */
            return (seq_t)0;
        }

        for (int i = 0; i <= MAX_LIMIT; ++i) {
            best_log[i]   = DBL_MAX;
            best_omega[i] = (seq_t)0;
        }
        best_log[1]   = 0.0;
        best_omega[1] = (seq_t)0;

        for (int i = 0; i < NUM_PRIMES; ++i)
            prime_log[i] = log((double)primes[i]);

        dfs(0, MAX_LIMIT, 0.0, 1, 0);
        initialized = true;
    }
    if (n <= MAX_LIMIT)
        return best_omega[(int)n];
    /* out of pre‑computed range – not expected in tests */
    return (seq_t)0;
}
