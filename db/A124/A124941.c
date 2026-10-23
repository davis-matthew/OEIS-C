/*
 * A124941  Numbers k such that k and k+4 are 4-almost primes.
 * https://oeis.org/A124941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1915, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th integer k such that k and k+4 are 4‑almost primes
 * (have exactly four prime factors with multiplicity).  The offset is 1. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define PRIME_SIEVE_LIMIT 20000000U          /* primes up to 20 000 000 */
#define MAX_TERMS        20000U              /* enough for n ≤ 10 000 */

static seq_t  _results[MAX_TERMS];
static size_t _results_used = 0;             /* number of terms stored */

static uint32_t _prime_table[1300000U];      /* space for primes < 20 M   */
static size_t   _prime_count  = 0;
static bool     _primes_ready = false;

static uint64_t _next_candidate = 2;          /* next value to test */

/* -------------------------------------------------------------
 * Build a sieve of primes up to PRIME_SIEVE_LIMIT.
 * ------------------------------------------------------------- */
static void _init_primes(void)
{
    if (_primes_ready) return;

    static unsigned char is_composite[PRIME_SIEVE_LIMIT + 1] = {0};
    size_t count = 0;

    for (uint32_t p = 2; p <= PRIME_SIEVE_LIMIT; ++p) {
        if (is_composite[p]) continue;
        _prime_table[count++] = p;
        if ((uint64_t)p * p <= PRIME_SIEVE_LIMIT) {
            for (uint64_t q = (uint64_t)p * p; q <= PRIME_SIEVE_LIMIT; q += p)
                is_composite[(size_t)q] = 1;
        }
    }
    _prime_count  = count;
    _primes_ready = true;
}

/* -------------------------------------------------------------
 * Count prime factors with multiplicity of a 64‑bit integer n.
 * ------------------------------------------------------------- */
static int _count_factors(uint64_t n)
{
    if (n <= 1) return 0;
    int count = 0;
    for (size_t i = 0; i < _prime_count; ++i) {
        uint32_t p = _prime_table[i];
        if ((uint64_t)p * p > n) break;
        while (n % p == 0) {
            ++count;
            n /= p;
        }
    }
    if (n > 1) ++count;    /* remaining prime factor */
    return count;
}

/* -------------------------------------------------------------
 * Store a new value in the static result array.
 * ------------------------------------------------------------- */
static void _add_result(seq_t val)
{
    if (_results_used < MAX_TERMS) {
        _results[_results_used++] = val;
    }
}

/* -------------------------------------------------------------
 * Main required function.
 * ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */

    _init_primes();

    if ((idx_t)_results_used >= n) {
        return _results[n - 1];
    }

    /* Scan candidates until we have at least n terms. */
    while ((idx_t)_results_used < n) {
        uint64_t k = _next_candidate++;
        if (_count_factors(k) == 4 && _count_factors(k + 4) == 4) {
            _add_result((seq_t)k);
        }
    }

    return _results[n - 1];
}
