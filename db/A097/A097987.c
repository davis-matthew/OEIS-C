/*
 * A097987  Numbers k such that 4 does not divide phi(k), where phi is Euler's
 *          totient function (A000010).
 * https://oeis.org/A097987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 5134, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097987: numbers k such that 4 does not divide φ(k).
   The sequence consists of 1, 2, 4, every odd prime power p^k
   with p ≡ 3 (mod 4), and every double of such a prime power
   2·p^k (k ≥ 1).                                           */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Static cache of computed terms ---------- */
static size_t _terms_size = 0;   /* number of stored terms      */
static size_t _terms_cap  = 0;   /* capacity of the array      */
static seq_t *_terms       = NULL;  /* array of terms             */

/* ---------- Next integer to examine ---------- */
static uint64_t _next_k = 0;

/* ---------- Prime table (1 000 000) ---------- */
static bool _primes_ready = false;
static int  *_primes      = NULL;   /* prime numbers             */
static size_t _primes_cnt = 0;      /* number of primes in table */

/* Build a list of all primes up to 1 000 000. */
static void _build_primes(void)
{
    if (_primes_ready) return;

    const uint64_t LIMIT = 1000000ULL;              /* upper bound for primes */
    size_t sz = (size_t)(LIMIT + 1);

    bool *is_prime = (bool *)calloc(sz, sizeof(bool));
    if (!is_prime) exit(1);

    /* Initially mark all >1 as prime. */
    for (uint64_t i = 2; i <= LIMIT; ++i) is_prime[i] = true;

    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes. */
    _primes_cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++_primes_cnt;

    _primes = (int *)malloc(_primes_cnt * sizeof(int));
    if (!_primes) { free(is_prime); exit(1); }

    size_t idx = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) _primes[idx++] = (int)i;

    free(is_prime);
    _primes_ready = true;
}

/* Compute φ(n) for n ≤ 2⁶⁴.  Uses the global prime list. */
static uint64_t _phi_u64(uint64_t n)
{
    if (n == 0) return 0;

    _build_primes();              /* ensure primes are ready */

    uint64_t result = n;
    for (size_t i = 0; i < _primes_cnt &&
                         (uint64_t)_primes[i] * (uint64_t)_primes[i] <= n; ++i) {
        uint64_t p = (uint64_t)_primes[i];
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1)                    /* remaining prime factor > sqrt(original n) */
        result -= result / n;
    return result;
}

/* Append a new term to the cache, growing if necessary. */
static void _append_term(seq_t value)
{
    if (_terms_size == _terms_cap) {
        size_t new_cap = _terms_cap == 0 ? 16 : _terms_cap * 2;
        seq_t *tmp = (seq_t *)realloc(_terms, new_cap * sizeof(seq_t));
        if (!tmp) { free(_terms); exit(1); }
        _terms = tmp;
        _terms_cap = new_cap;
    }
    _terms[_terms_size++] = value;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but harmless */

    /* Generate terms until we have at least n of them. */
    while (_terms_size < (size_t)n) {
        ++_next_k;                         /* next integer to test */

        uint64_t phi_val = _phi_u64(_next_k);

        if (phi_val % 4 != 0) {            /* 4 ∤ φ(k)  */
            _append_term((seq_t)_next_k);
        }
    }

    return _terms[(size_t)n - 1];
}
