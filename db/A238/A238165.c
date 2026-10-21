/*
 * A238165  Number of pairs {j, k} with 0 < j < k <= n such that pi(j*n)
 *          divides pi(k*n), where pi(.) is given by A000720.
 * https://oeis.org/A238165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #pairs {j,k} with 0 < j < k ≤ n such that pi(j*n) divides
 *            pi(k*n), where pi is the prime‑counting function
 * Range:     u8 n <= 41, u16 n <= 400, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* static storage for a dynamic sieve */
static bool _sieve_ready = false;
static idx_t _sieve_limit = 0;
static uint8_t *_is_prime = NULL;   /* 0 or 1 for each integer up to _sieve_limit */
static uint32_t *_pi_prefix = NULL; /* pi(x) for x ≤ _sieve_limit */

/* rebuild or extend the sieve up to 'limit' */
static void _ensure_sieve(idx_t limit)
{
    if (_sieve_ready && _sieve_limit >= limit)
        return;

    /* free old arrays if they exist */
    if (_sieve_ready) {
        free(_is_prime);
        free(_pi_prefix);
    }

    /* allocate new arrays */
    _is_prime = (uint8_t *)malloc((size_t)(limit + 1));
    _pi_prefix = (uint32_t *)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    if (!_is_prime || !_pi_prefix)
        return; /* allocation failed – programme will fail gracefully */

    /* initialise all entries as prime */
    for (size_t i = 0; i <= (size_t)limit; ++i)
        _is_prime[i] = 1;
    _is_prime[0] = _is_prime[1] = 0;

    /* classic Eratosthenes sieve */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (_is_prime[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                _is_prime[(size_t)j] = 0;
        }
    }

    /* build prefix sum of primes (pi function) */
    uint32_t acc = 0;
    _pi_prefix[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if (_is_prime[(size_t)i])
            ++acc;
        _pi_prefix[(size_t)i] = acc;
    }

    _sieve_limit = limit;
    _sieve_ready = true;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* need primes up to n*n */
    idx_t limit = n * n;
    _ensure_sieve(limit);

    /* allocate temporary array for pi(j*n) */
#ifdef __cplusplus
    seq_t *P = new seq_t[n + 1];
#else
    seq_t *P = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
#endif
    if (!P)
        return 0; /* memory failure, return 0 */

    /* fill P[j] with pi(j*n) */
    for (idx_t j = 1; j <= n; ++j) {
        idx_t pos = j * n;
        P[(size_t)j] = (seq_t)_pi_prefix[(size_t)pos];
    }

    /* count qualifying pairs */
    seq_t count = 0;
    for (idx_t j = 1; j <= n - 1; ++j) {
        seq_t pj = P[(size_t)j];
        if (pj == 1) {
            /* 1 divides every later value */
            count += (seq_t)(n - j);
            continue;
        }
        for (idx_t k = j + 1; k <= n; ++k) {
            seq_t pk = P[(size_t)k];
            if (pk % pj == 0)
                ++count;
        }
    }

    /* free temporary array */
#ifdef __cplusplus
    delete[] P;
#else
    free(P);
#endif

    return count;
}
