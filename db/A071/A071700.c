/*
 * A071700  Product of twin primes of form (4*k+3,4*(k+1)+1), k>=0.
 * https://oeis.org/A071700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 437, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A071700: a(n) = product of the n‑th twin prime pair of the form
 * (4k+3,4k+5).  The term is found by searching successive candidates p = 4k+3
 * and testing both p and p+2 for primality.  No simple formula is known.  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ----------  Deterministic Miller–Rabin test for uint64_t  ---------- */

static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static inline uint64_t mod_pow(uint64_t a, uint64_t d, uint64_t m)
{
    uint64_t r = 1;
    while (d)
    {
        if (d & 1) r = mod_mul(r, a, m);
        a    = mod_mul(a, a, m);
        d >>= 1;
    }
    return r;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)            return false;
    if (n < 4)            return true;         /* 2,3 */
    if ((n & 1) == 0)     return false;        /* even */

    static const uint64_t small_primes[] = {
        3,5,7,11,13,17,19,23,29,31,37
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* bases that guarantee determinism for 64‑bit numbers */
    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i];
        if (a >= n) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ----------  Sequence storage and generation  ---------- */

static uint64_t      _twin_candidate;   /* current 4k+3 candidate         */
static idx_t         _terms_generated;  /* number of terms already stored */
static seq_t       * _terms_cache;      /* dynamic array of results       */
static size_t       _terms_cap;        /* current capacity of the cache  */

/* enlarge the cache to hold at least new_cap elements */
static void _ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= _terms_cap) return;
    size_t cap = _terms_cap ? _terms_cap : 64;
    while (cap < new_cap) cap <<= 1;
    seq_t *new_mem = (seq_t *)realloc(_terms_cache, cap * sizeof(seq_t));
    if (!new_mem) {
        /* out of memory – terminate the program */
        exit(1);
    }
    _terms_cache = new_mem;
    _terms_cap   = cap;
}

/* generate terms until n terms are stored */
static void _generate_n_terms(idx_t n)
{
    while (_terms_generated < n)
    {
        uint64_t p = _twin_candidate;
        uint64_t q = p + 2;
        if (is_prime_uint64(p) && is_prime_uint64(q))
        {
            seq_t prod = (seq_t)p * (seq_t)q;
            _ensure_cache_capacity(_terms_generated + 1);
            _terms_cache[_terms_generated] = prod;
            ++_terms_generated;
        }
        _twin_candidate += 4; /* next candidate of the form 4k+3 */
    }
}

/* ----------  Public entry point  ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* offset is 1 */

    if (!_terms_cache)                         /* first call – initialise */
    {
        _twin_candidate  = 3;                   /* first 4k+3 = 3   */
        _terms_generated = 0;
        _terms_cache     = NULL;
        _terms_cap       = 0;
    }

    _generate_n_terms(n);
    return _terms_cache[n-1];
}
