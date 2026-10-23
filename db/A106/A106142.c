/*
 * A106142  Semiprimes (mod 6).
 * https://oeis.org/A106142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = residue (mod 6) of the n‑th semiprime (product of two
 *            primes, counted with multiplicity). For n ≥ 1. Implemented by
 *            sieving for all semiprimes up to a fixed limit and extending the
 *            list lazily for larger n
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#if defined(__cplusplus)
#include <new>                     /* for placement new in C++  */
#endif

/* ---------- configuration -------------------------------------------------- */
#define SEMI_LIMIT   2000000U        /* pre‑compute all semiprimes ≤ LIMIT          */

/* ---------- static state --------------------------------------------------- */
static int          _a_initialized = 0;      /* flag for one‑time init          */
static uint32_t    *_a_spf      = NULL; /* smallest prime factor array          */
static uint32_t    *_a_primes   = NULL; /* list of primes                       */
static size_t       _a_prime_cnt  = 0;
static size_t       _a_prime_cap  = 0;

static seq_t       *_a_sem_residue = NULL; /* residues of semiprimes          */
static size_t       _a_sem_cnt     = 0;    /* number of stored residues         */
static size_t       _a_sem_cap     = 0;    /* current capacity                  */

/* ---------- prime list helpers -------------------------------------------- */
static void _a_extend_primes(void)
{
    size_t new_cap = (_a_prime_cap == 0) ? 1024 : (_a_prime_cap << 1);
    uint32_t *new_arr = (uint32_t *)realloc(_a_primes, new_cap * sizeof(uint32_t));
    if (!new_arr) abort();
    _a_primes = new_arr;
    _a_prime_cap = new_cap;
}

/* ---------- semiprime array helpers --------------------------------------- */
static void _a_extend_sem(void)
{
    size_t old_cap = _a_sem_cap;
    _a_sem_cap = (_a_sem_cap == 0) ? 1024 : (_a_sem_cap << 1);
    seq_t *new_arr = (seq_t *)realloc(_a_sem_residue, _a_sem_cap * sizeof(seq_t));
    if (!new_arr) abort();
    _a_sem_residue = new_arr;
#if defined(__cplusplus)
    /* default‑construct newly allocated elements in C++ */
    for (size_t i = old_cap; i < _a_sem_cap; ++i)
        new (&_a_sem_residue[i]) seq_t();
#endif
}

/* ---------- initialization -------------------------------------------------- */
static void _a_initialize(void)
{
    /* ----- small prime factor sieve ------------------------------------- */
    _a_spf = (uint32_t *)calloc(SEMI_LIMIT + 1, sizeof(uint32_t));
    if (!_a_spf) abort();

    _a_prime_cap = 1024;
    _a_primes = (uint32_t *)malloc(_a_prime_cap * sizeof(uint32_t));
    if (!_a_primes) abort();

    for (uint32_t i = 2; i <= SEMI_LIMIT; ++i) {
        if (_a_spf[i] == 0) {
            _a_spf[i] = i;
            if (_a_prime_cnt == _a_prime_cap) _a_extend_primes();
            _a_primes[_a_prime_cnt++] = i;
        }
        for (size_t j = 0; j < _a_prime_cnt; ++j) {
            uint32_t p = _a_primes[j];
            uint64_t v = (uint64_t)i * p;
            if (v > SEMI_LIMIT) break;
            _a_spf[(uint32_t)v] = p;
            if (p == _a_spf[i]) break;
        }
    }

    /* ----- pre‑compute residues of all semiprimes ≤ SEMI_LIMIT ------------ */
    _a_sem_cap = 1024;
    _a_sem_residue = (seq_t *)malloc(_a_sem_cap * sizeof(seq_t));
    if (!_a_sem_residue) abort();

    for (uint32_t n = 4; n <= SEMI_LIMIT; ++n) {
        uint32_t p = _a_spf[n];
        uint32_t q = n / p;
        if (_a_spf[q] == q) {           /* q is prime → n is semiprime */
            if (_a_sem_cnt == _a_sem_cap) _a_extend_sem();
#if defined(__cplusplus)
            new (&_a_sem_residue[_a_sem_cnt]) seq_t(n % 6);
#else
            _a_sem_residue[_a_sem_cnt] = (seq_t)(n % 6);
#endif
            ++_a_sem_cnt;
        }
    }

    _a_initialized = 1;
}

/* ---------- prime test for numbers > SEMI_LIMIT --------------------------- */
static int _a_is_prime_big(uint64_t x)
{
    if (x < 2) return 0;
    for (size_t i = 0; i < _a_prime_cnt; ++i) {
        uint32_t p = _a_primes[i];
        if ((uint64_t)p * p > x) break;
        if (x % p == 0) return 0;
    }
    return 1;
}

/* ---------- grow the list of residues up to a requested index ------------- */
static void _a_ensure_terms(idx_t n)
{
    /* n is 1‑based */
    if (n <= _a_sem_cnt) return;

    static uint64_t next_candidate = SEMI_LIMIT + 1;

    while (_a_sem_cnt < (size_t)n) {
        uint64_t x = next_candidate;
        ++next_candidate;

        if (x < 4) continue;

        int is_semiprime = 0;
        if (x <= SEMI_LIMIT) {
            uint32_t p = _a_spf[(uint32_t)x];
            uint32_t q = (uint32_t)(x / p);
            if (_a_spf[q] == q) is_semiprime = 1;
        } else {
            for (size_t i = 0; i < _a_prime_cnt; ++i) {
                uint32_t p = _a_primes[i];
                uint64_t psq = (uint64_t)p * p;
                if (psq > x) break;
                if (x % p == 0) {
                    uint64_t q = x / p;
                    if (_a_is_prime_big(q)) { is_semiprime = 1; break; }
                }
            }
        }

        if (is_semiprime) {
            if (_a_sem_cnt == _a_sem_cap) _a_extend_sem();
#if defined(__cplusplus)
            new (&_a_sem_residue[_a_sem_cnt]) seq_t(x % 6);
#else
            _a_sem_residue[_a_sem_cnt] = (seq_t)(x % 6);
#endif
            ++_a_sem_cnt;
        }
    }
}

/* ---------- public entry -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!_a_initialized) _a_initialize();
    _a_ensure_terms(n);
    return _a_sem_residue[n - 1];
}
