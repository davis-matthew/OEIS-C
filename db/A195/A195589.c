/*
 * A195589  G.f.: x/exp( Sum_{n>=1} a(n)*x^n/n ) = Sum_{n>=1} moebius(n)*x^n.
 * https://oeis.org/A195589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = - n*μ(n+1) - ∑_{k=1}^{n-1} a(k) * μ(n-k+1) where μ is the
 *            Möbius function. The sequence is the Witt transform of the
 *            Möbius series. This implementation precomputes μ values up to
 *            the requested index and uses dynamic programming to compute a(n)
 *            on demand, working for any build where seq_t i...
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*-----------------  Möbius function up to a limit  -----------------*/
static size_t _mu_cap = 0;
static int8_t * _mu = NULL;  /* _mu[0] unused, _mu[1] = 1 */

static void _ensure_mu(idx_t limit)
{
    if (_mu_cap > limit) return;             /* already computed */

    /* Allocate space for new entries */
    size_t new_cap = (size_t)limit + 1;
    _mu = (int8_t *)realloc(_mu, new_cap * sizeof(int8_t));
    if (!_mu) abort();

    /* Linear sieve for Möbius function */
    bool *is_comp = (bool *)calloc(new_cap, sizeof(bool));
    if (!is_comp) abort();
    int *primes = (int *)malloc(new_cap * sizeof(int));
    if (!primes) abort();
    size_t primes_cnt = 0;

    _mu[0] = 0;
    _mu[1] = 1;

    for (size_t i = 2; i < new_cap; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = (int)i;
            _mu[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            size_t p = (size_t)primes[j];
            size_t m = i * p;
            if (m >= new_cap) break;
            is_comp[m] = true;
            if (i % p == 0) {
                _mu[m] = 0;
                break;
            } else {
                _mu[m] = -(int8_t)_mu[i];
            }
        }
    }

    free(is_comp);
    free(primes);
    _mu_cap = limit;
}

/*-----------------  Sequence a[n] (A195589) -----------------*/
static size_t _a_cap = 0;
static seq_t * _a = NULL;   /* 1‑based, _a[0] unused or 0 */
static idx_t _max_a = 0;

static void _ensure_a(idx_t limit)
{
    if (_max_a > limit) return;          /* already computed */

    _ensure_mu(limit + 1);               /* need μ up to n+1 */

    if (_a_cap <= (size_t)limit) {
        size_t new_cap = (_a_cap > 0) ? _a_cap : 16;
        while (new_cap <= (size_t)limit) new_cap <<= 1;
        _a = (seq_t *)realloc(_a, new_cap * sizeof(seq_t));
        if (!_a) abort();
        _a_cap = new_cap;
    }

    if (_max_a == 0) {
        _a[0] = 0;     /* unused */
        _a[1] = (seq_t)1;
        _max_a = 1;
    }

    /* Compute terms that are not yet known */
#if defined(__SIZEOF_INT128__)
    typedef __int128 signed128;
#else
    typedef long long signed128;
#endif

    for (idx_t n = _max_a + 1; n <= limit; ++n) {
        signed128 val = -(signed128)n * (signed128)_mu[(size_t)(n + 1)];
        for (idx_t k = 1; k < n; ++k) {
            int8_t mu_val = _mu[(size_t)(n - k + 1)];
            if (mu_val == -1) {
                val += (signed128)_a[(size_t)k];
            } else if (mu_val == 1) {
                val -= (signed128)_a[(size_t)k];
            }
        }
        /* The result should be non‑negative */
        _a[(size_t)n] = (seq_t)val;
    }

    _max_a = limit;
}

/*-----------------  Public API -----------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */
    _ensure_a(n);
    return _a[(size_t)n];
}
