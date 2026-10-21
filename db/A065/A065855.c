/*
 * A065855  Number of composites <= n.
 * https://oeis.org/A065855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = n - pi(n) - 1
 * Range:     u8 n <= 322, u16 n <= 9793, u32 n <= 9793, u64 n <= 9793,
 *            u128 n <= 9793, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) counts the composites less than or equal to n for n >= 1   */

#include <stdlib.h>

/* Internal state for the sieve and prime‑count prefix sums */
static idx_t  _max_n    = 0;
static uint8_t * _is_prime = NULL;   /* 1 = prime, 0 = composite */
static idx_t *   _pi_cache = NULL;   /* _pi_cache[i] = pi(i) */

/* Build or enlarge the sieve up to the given maximum n (inclusive) */
static void _ensure_up_to(idx_t up)
{
    if (up <= _max_n) return;                     /* already built */

    size_t new_size = (size_t)up + 1;             /* array length */
    _is_prime = (uint8_t *)realloc(_is_prime, new_size);
    _pi_cache = (idx_t *)realloc(_pi_cache, new_size * sizeof(idx_t));
    if (!_is_prime || !_pi_cache)                /* allocation failed */
        abort();

    /* Initialise all as prime, except 0 and 1 */
    for (size_t i = 0; i < new_size; ++i) _is_prime[i] = 1;
    if (new_size > 0) _is_prime[0] = 0;
    if (new_size > 1) _is_prime[1] = 0;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= up; ++i) {
        if (_is_prime[(size_t)i]) {
            for (idx_t j = i * i; j <= up; j += i)
                _is_prime[(size_t)j] = 0;
        }
    }

    /* Build prefix sums of prime counts (pi values) */
    idx_t acc = 0;
    _pi_cache[0] = 0;
    for (idx_t i = 1; i <= up; ++i) {
        if (_is_prime[(size_t)i]) ++acc;
        _pi_cache[(size_t)i] = acc;
    }

    _max_n = up;
}

/* Required sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* a(1) = 0 is the first defined term */
    _ensure_up_to(n);
    idx_t pi = _pi_cache[(size_t)n];
    return (seq_t)n - (seq_t)pi - 1;
}
