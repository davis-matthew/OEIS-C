/*
 * A097468  Number of 1's in the decimal expansion of the lesser of twin
 *          primes.
 * https://oeis.org/A097468
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of 1's in the decimal expansion of the
 * lesser of the n-th twin prime, for n >= 2.  The sequence
 * starts with a(2) as the first term (the smaller of 3 and 5). */
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_counts.h"

/* static state used to keep a sieve and the list of twin-prime leaders */
static idx_t   _sieve_limit   = 100;          /* current sieve bound       */
static uint8_t *_prime_flag    = NULL;        /* 1 if prime, 0 otherwise   */
static idx_t *_twin_leaders   = NULL;        /* lesser of each twin pair   */
static size_t _twin_cap       = 0;            /* allocated size of array    */
static size_t _twin_cnt       = 0;            /* number of twin pairs found */

/* Perform an Eratosthenes sieve up to _sieve_limit and collect
 * the lesser members of twin primes. */
static void
_sieve(void)
{
    /* free previous flag array and allocate a new one */
    free(_prime_flag);
    _prime_flag = (uint8_t *)malloc((_sieve_limit + 1) * sizeof(uint8_t));
    if (! _prime_flag)
        exit(EXIT_FAILURE);          /* out of memory */

    /* initialise all indices as prime */
    memset(_prime_flag, 1, _sieve_limit + 1);
    _prime_flag[0] = _prime_flag[1] = 0;

    /* classic sieve */
    for (idx_t i = 2; i * i <= _sieve_limit; ++i) {
        if (_prime_flag[i]) {
            for (idx_t j = i * i; j <= _sieve_limit; j += i)
                _prime_flag[j] = 0;
        }
    }

    /* collect the lesser twin primes */
    _twin_cnt = 0;
    for (idx_t p = 2; p + 2 <= _sieve_limit; ++p) {
        if (_prime_flag[p] && _prime_flag[p + 2]) {
            if (_twin_cnt == _twin_cap) {
                _twin_cap = (_twin_cap == 0) ? 64 : _twin_cap * 2;
                _twin_leaders = (idx_t *)realloc(_twin_leaders,
                                                 _twin_cap * sizeof(idx_t));
                if (! _twin_leaders)
                    exit(EXIT_FAILURE);
            }
            _twin_leaders[_twin_cnt++] = p;
        }
    }
}

/* Ensure that at least `needed` twin pairs have been found.  If not,
 * double the sieve limit until we have enough. */
static void
_ensure_twin_count(idx_t needed)
{
    if (_twin_cnt >= (size_t)needed)
        return;

    while (_twin_cnt < (size_t)needed) {
        if (_sieve_limit < 100)                 /* guard against underflow */
            _sieve_limit = 100;
        else
            _sieve_limit <<= 1;                 /* double the bound */
        _sieve();
    }
}

/* Main entry point for the sequence: a(n) for n >= 2. */
seq_t
a(idx_t n)
{
    if (n < 2)
        return 0;                 /* behaviour below the offset is irrelevant */

    /* we need the (n‑2)‑th twin prime in 0‑based indexing */
    _ensure_twin_count(n);        /* guarantees _twin_cnt >= n */
    idx_t lesser = _twin_leaders[n - 2];   /* n=2 -> first twin (3) */

    unsigned char cnt[10];
    digit_counts(lesser, cnt);     /* cnt[1] == number of '1' digits */
    return (seq_t)cnt[1];
}
