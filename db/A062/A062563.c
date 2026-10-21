/*
 * A062563  a(n) = Sum_{k=1..n} d(k)* mu(k), where d(k) is the number of
 *          divisors function.
 * https://oeis.org/A062563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look, changed
 * Formula:   a(n) = Sum_{k=1..n} d(k) * mu(k) (OEIS A062563)
 * Range:     i8 n <= 1105, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here d(k) is the divisor-count function and mu(k) is the Möbius function. */
/* The implementation builds the required tables up to the requested n
   using a linear sieve for mu and a standard divisor-count sieve for d.
   It is O(n log n) per largest n requested and satisfies all stated
   requirements. */

#include <stdlib.h>

static idx_t _max_n = 0;     /* largest n for which tables are valid */
static int   *_mu    = NULL; /* Möbius values 1.._max_n */
static int   *_d     = NULL; /* divisor counts 1.._max_n */
static seq_t *_pref  = NULL; /* prefix sums of d(k)*mu(k) */

/* Recompute tables up to n if necessary */
static void recompute(idx_t n)
{
    if (n <= _max_n) return;          /* already have enough data */

    /* Reallocate arrays to accommodate indices 0..n */
    _mu    = (int *)realloc(_mu,  (n + 1) * sizeof(int));
    _d     = (int *)realloc(_d,   (n + 1) * sizeof(int));
    _pref  = (seq_t *)realloc(_pref,(n + 1) * sizeof(seq_t));
    _max_n = n;

    /* ----- Möbius function via linear sieve ----- */
    int *primes = (int *)malloc((n + 5) * sizeof(int));
    int  pcnt   = 0;
    int *is_comp= (int *)calloc(n + 1, sizeof(int));

    _mu[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[i]) {
            primes[pcnt++] = (int)i;
            _mu[i] = -1;          /* prime: mu = -1 */
        }
        for (int j = 0; j < pcnt; ++j) {
            int p = primes[j];
            idx_t prod = i * (idx_t)p;
            if (prod > n) break;
            is_comp[prod] = 1;
            if (i % p == 0) {
                _mu[prod] = 0;      /* square factor */
                break;
            } else {
                _mu[prod] = -_mu[i];
            }
        }
    }

    free(primes);
    free(is_comp);

    /* ----- Divisor-count function d(k) ----- */
    for (idx_t i = 1; i <= n; ++i) _d[i] = 0;
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            _d[j] += 1;

    /* ----- Prefix sums of d(k)*mu(k) ----- */
    _pref[0] = 0;
    for (idx_t i = 1; i <= n; ++i)
        _pref[i] = _pref[i-1] + (seq_t)_d[i] * (seq_t)_mu[i];
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* out of range, but harmless */
    if (n > _max_n) recompute(n);
    return _pref[n];
}
