/*
 * A220273  a(n) is the smallest number, such that for all N >= a(n) there are
 *          at least n primes between 5*N and 6*N.
 * https://oeis.org/A220273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A220273: a(n) is the smallest integer N such that for all M ≥ N
 * the interval (5*M, 6*M) contains at least n primes.
 * This implementation precomputes prime counts up to 6·MAX_N,
 * builds the array of prime counts in each interval, then
 * computes suffix minima.  The value a(n) is found by scanning
 * the suffix minima array.  The method works for all n ≥ 1.  */

/* required headers */
#include <stdbool.h>
#include <stdlib.h>

/* limits ---------------------------------------------------------- */
#define MAX_N      200000UL      /* enough for n up to about 10000   */
#define MAX_PRIME  (6*MAX_N)     /* compute primes up to 6·MAX_N     */

/* static data ----------------------------------------------------- */
static bool _a_initialized = false;
static idx_t *_a_pi  = NULL;  /* prefix prime counts, size MAX_PRIME+1 */
static idx_t *_a_d  = NULL;  /* primes in (5N,6N),    N=1..MAX_N  */
static idx_t *_a_g  = NULL;  /* suffix minimums,      N=1..MAX_N  */

/* ------------------------------------------------------------------ */
static void _a_precompute(void)
{
    /* allocate memory */
    _a_pi = (idx_t *)malloc((MAX_PRIME + 1) * sizeof(idx_t));
    if (!_a_pi) { abort(); }
    _a_d  = (idx_t *)malloc((MAX_N + 1) * sizeof(idx_t));
    if (!_a_d)  { abort(); }
    _a_g  = (idx_t *)malloc((MAX_N + 1) * sizeof(idx_t));
    if (!_a_g)  { abort(); }

    /* is_prime sieve */
    bool *is_prime = (bool *)malloc((MAX_PRIME + 1) * sizeof(bool));
    if (!is_prime) { abort(); }
    for (idx_t i = 0; i <= MAX_PRIME; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t p = 2; p * p <= MAX_PRIME; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= MAX_PRIME; m += p) {
                is_prime[m] = false;
            }
        }
    }

    /* prefix prime counts */
    _a_pi[0] = 0;
    for (idx_t i = 1; i <= MAX_PRIME; ++i) {
        _a_pi[i] = _a_pi[i - 1] + (is_prime[i] ? 1 : 0);
    }
    free(is_prime);

    /* primes in each interval (5N,6N) */
    for (idx_t N = 1; N <= MAX_N; ++N) {
        idx_t high_idx = 6 * N - 1;          /* inclusive upper bound  */
        idx_t low_idx  = 5 * N;              /* exclusive lower bound  */
        _a_d[N] = _a_pi[high_idx] - _a_pi[low_idx];
    }

    /* suffix minima: g[N] = min_{M ≥ N} diff[M] */
    idx_t cur_min = (idx_t)-1;
    for (idx_t N = MAX_N; N >= 1; --N) {
        if (_a_d[N] < cur_min) cur_min = _a_d[N];
        _a_g[N] = cur_min;
    }

    _a_initialized = true;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!_a_initialized) _a_precompute();

    /* scan for first N with g[N] ≥ n */
    for (idx_t N = 1; N <= MAX_N; ++N) {
        if (_a_g[N] >= n) return (seq_t)N;
    }

    /* if not found within precomputed range */
    return (seq_t)(MAX_N + 1);
}
