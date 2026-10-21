/*
 * A063466  Numbers k such that 2*phi(k) + 8 = sigma(k).
 * https://oeis.org/A063466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * prime(n) for n > 16 a(1..16) = {6, 15, 21, 33, 39,
 *            51, 57, 69, 87, 93, 111, 123, 129, 141, 159, 175}
 * Range:     u8 n <= 23, u16 n <= 2450, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

static const seq_t first16[16] = {
    6, 15, 21, 33, 39, 51, 57, 69,
    87, 93, 111, 123, 129, 141, 159, 175
};

/* --- Prime generation utilities ---------------------------------- */
static idx_t _prime_cnt     = 0;          /* number of primes stored */
static seq_t * _prime_arr   = NULL;      /* dynamic array of primes */
static idx_t _prime_cap     = 0;          /* allocated capacity */
static idx_t _next_candidate = 2;         /* integer to test next */

/* Allocate or grow the prime array to hold at least 'need' primes */
static void _ensure_prime_count(idx_t need)
{
    if (_prime_cnt >= need)
        return;

    /* Grow capacity to a reasonable size (double) */
    if (_prime_cap < need) {
        size_t new_cap = (_prime_cap == 0 ? 16 : _prime_cap << 1);
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(_prime_arr, new_cap * sizeof(seq_t));
        if (!tmp)
            return;                /* out of memory – silently fail */
        _prime_arr = tmp;
        _prime_cap = new_cap;
    }

    /* Find primes until we have 'need' of them */
    while (_prime_cnt < need) {
        idx_t candidate = _next_candidate++;

        /* Test divisibility by existing primes up to sqrt(candidate) */
        idx_t lim = (idx_t)floor(sqrt((double)candidate));
        int isprime = 1;
        for (idx_t i = 0; i < _prime_cnt; ++i) {
            seq_t p = _prime_arr[i];
            if (p > lim)
                break;
            if (candidate % p == 0) {
                isprime = 0;
                break;
            }
        }

        if (isprime) {
            _prime_arr[_prime_cnt++] = (seq_t)candidate;
        }
    }
}

/* Return the n-th prime (1‑based). If out of bounds, returns 0. */
static seq_t _nth_prime(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    _ensure_prime_count(n);
    return _prime_arr[n - 1];
}

/* --- Main sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined: return 0 */
        return (seq_t)0;
    if (n <= 16)
        return first16[n - 1];

    /* For n > 16: a(n) = 3 * prime(n) */
    seq_t p = _nth_prime(n);
    return p * 3;
}
