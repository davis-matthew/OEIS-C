/*
 * A037229  Numbers k such that pi(k) >= phi(k).
 * https://oeis.org/A037229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A037229: numbers k such that pi(k) >= phi(k) */
#include <stddef.h>
#include <stdbool.h>

#define LIMIT     6000      /* all terms lie below this bound */
#define MAX_TERMS 128       /* enough for all known terms */

static bool _initialized = false;

static unsigned char  _is_prime[LIMIT + 1];
static unsigned int   _phi[LIMIT + 1];
static unsigned int   _pi[LIMIT + 1];
static seq_t          _terms[MAX_TERMS];
static size_t         _term_cnt = 0;

static void _init(void)
{
    if (_initialized) return;
    /* --- Sieve for primes up to LIMIT --- */
    for (size_t i = 0; i <= LIMIT; ++i) _is_prime[i] = 1;
    _is_prime[0] = _is_prime[1] = 0;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (_is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                _is_prime[j] = 0;
        }
    }

    /* --- Euler totient sieve --- */
    for (size_t i = 0; i <= LIMIT; ++i) _phi[i] = (unsigned int)i;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (_is_prime[i]) {
            for (size_t j = i; j <= LIMIT; j += i) {
                _phi[j] -= _phi[j] / (unsigned int)i;
            }
        }
    }

    /* --- Prime counting function pi(k) --- */
    _pi[0] = 0;
    for (size_t i = 1; i <= LIMIT; ++i) {
        _pi[i] = _pi[i - 1] + (_is_prime[i] ? 1 : 0);
    }

    /* --- Gather all terms that satisfy pi(k) >= phi(k) --- */
    _term_cnt = 0;
    for (size_t k = 2; k <= LIMIT; ++k) {
        if ((seq_t)_pi[k] >= (seq_t)_phi[k]) {
            if (_term_cnt < MAX_TERMS) {
                _terms[_term_cnt++] = (seq_t)k;
            }
        }
    }

    _initialized = true;
}

seq_t a(idx_t n)
{
    _init();
    if ((size_t)n < _term_cnt) return _terms[(size_t)n];
    return (seq_t)0;   /* undefined beyond precomputed range */
}
