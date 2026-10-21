/*
 * A206818  Position of n+(n+1)/log(n+1) in the joint ranking of {j+pi(j)} and
 *          {k+(k+1)/log(k+1)}.
 * https://oeis.org/A206818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + # { j | j + pi(j) <= n + (n+1)/log(n+1) }
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes this by generating pi(j) up to a bound that
   guarantees all relevant j are stored.  It uses double precision
   only for the evaluation of the real expression B(n) = n + (n+1)/log(n+1). */

/* The following headers are needed for size_t, bool, and the log() function. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Utility: Sieve of Eratosthenes up to N, returning a table of cumulative
   prime counts pi[i] (number of primes <= i).  The table is stored in
   static arrays that are resized when a larger limit is needed. */
static size_t _sieve_limit = 0;
static uint8_t * _is_prime = NULL;
static uint32_t * _pi_cnt   = NULL;  /* pi_cnt[i] = pi(i) */

static void _ensure_sieve(size_t n)
{
    if (n <= _sieve_limit) return;              /* already large enough */

    /* free old tables if any */
    free(_is_prime);
    free(_pi_cnt);

    /* allocate new tables */
    _is_prime = (uint8_t*)malloc((n + 1) * sizeof(uint8_t));
    _pi_cnt   = (uint32_t*)malloc((n + 1) * sizeof(uint32_t));
    if (!_is_prime || !_pi_cnt) {
        /* allocation failed */
        exit(1);
    }

    /* initialize */
    for (size_t i = 0; i <= n; ++i) _is_prime[i] = 1;
    _is_prime[0] = _is_prime[1] = 0;

    /* sieve */
    for (size_t p = 2; p * p <= n; ++p) {
        if (_is_prime[p]) {
            for (size_t multiple = p * p; multiple <= n; multiple += p) {
                _is_prime[multiple] = 0;
            }
        }
    }

    /* build pi prefix */
    _pi_cnt[0] = 0;
    for (size_t i = 1; i <= n; ++i) {
        _pi_cnt[i] = _pi_cnt[i - 1] + (_is_prime[i] ? 1 : 0);
    }

    _sieve_limit = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 by the problem statement. */
    double nb = (double)n + (double)(n + 1) / log((double)(n + 1));

    /* Upper bound for j.  Any j > nb cannot satisfy j+pi(j) <= nb.
       An extra margin ensures we have all needed values. */
    size_t lim = (size_t)nb + 100;  /* +100 for safety */
    _ensure_sieve(lim);

    /* Count j with j + pi(j) <= nb. */
    size_t count = 0;
    for (size_t j = 1; j <= lim; ++j) {
        uint32_t pcount = _pi_cnt[j];
        double val = (double)j + (double)pcount;
        if (val <= nb + 1e-12) {            /* tiny epsilon to avoid FP errors */
            ++count;
        } else {
            break;                          /* further j will be larger */
        }
    }

    /* Return the position: n (for the B(n) term itself) + count of A terms. */
    return (seq_t)n + (seq_t)count;
}
