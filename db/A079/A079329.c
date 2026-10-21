/*
 * A079329  Let g(n)=A006512(n) be the larger member of the n-th pair of twin
 *          primes. Then a(n) is the average of g(n) and g(n+1).
 * https://oeis.org/A079329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (g(n) + g(n+1))/2 * where g(n) is the larger member of
 *            the n‑th twin‑prime pair. * The function lazily builds a list of
 *            twin‑prime big‑members * using trial division with a growing
 *            list of primes. * * The implementation is compatible with both
 *            C11 and C++17 * and uses only standard library...
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *_primes = NULL;      /* list of primes found so far */
static size_t _primes_len = 0;
static size_t _primes_cap = 0;

static uint64_t *_g = NULL;          /* list of twin‑prime big members (g values) */
static size_t _g_len = 0;
static size_t _g_cap = 0;

static uint64_t _next_candidate = 3; /* next odd number to test */

static int is_prime(uint64_t n)
{
    if (n < 2) return 0;
    for (size_t i = 0; i < _primes_len; ++i) {
        uint64_t p = _primes[i];
        if ((unsigned __int128)p * p > n) break;
        if (n % p == 0) return 0;
    }
    return 1;
}

/* append a prime to the _primes list */
static void add_prime(uint64_t p)
{
    if (_primes_len == _primes_cap) {
        size_t new_cap = _primes_cap ? _primes_cap * 2 : 8;
        uint64_t *tmp = (uint64_t *)realloc(_primes, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        _primes = tmp;
        _primes_cap = new_cap;
    }
    _primes[_primes_len++] = p;
}

/* append a twin‑prime big member to _g list */
static void add_g(uint64_t val)
{
    if (_g_len == _g_cap) {
        size_t new_cap = _g_cap ? _g_cap * 2 : 8;
        uint64_t *tmp = (uint64_t *)realloc(_g, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        _g = tmp;
        _g_cap = new_cap;
    }
    _g[_g_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but return 0 for safety */

    /* ensure the prime list contains 2 at the beginning */
    if (_primes_len == 0) {
        add_prime(2);
    }

    /* Generate twin‑prime big members until we have g(n) and g(n+1) */
    while (_g_len <= (size_t)n) { /* need indices 0..n inclusive */
        uint64_t p = _next_candidate;

        /* test p for primality */
        if (is_prime(p)) {
            /* check twin pair (p, p+2) */
            if (is_prime(p + 2)) {
                add_g(p + 2);
            }
            /* store p in prime list */
            add_prime(p);
        }

        _next_candidate += 2; /* skip even numbers */
    }

    /* compute a(n) = (g(n) + g(n+1)) / 2 */
    seq_t sum = (seq_t)_g[n - 1] + (seq_t)_g[n];
    return sum / 2;
}
