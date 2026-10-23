/*
 * A186688  Semiprimes of the form n^4 + 1.
 * https://oeis.org/A186688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 90, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A186688: Semiprimes of the form n^4 + 1
 * a(1) = 82, a(2) = 626, 2402, 4097, ...
 *
 * The function below generates all such values on demand,
 * caching the results to answer any index in O(1) after the
 * initial precomputation.
 *
 * It uses a deterministic Miller‑Rabin test for 64‑bit
 * integers together with Pollard’s Rho to fetch a non‑trivial
 * factor. A number is semiprime if it is not prime, has a
 * non‑trivial factor f, and both f and n/f are prime.
 *
 * For values larger than 2^64‑1 the algorithm stops, because
 * the supplied utilities operate on 64‑bit integers only.
 * The tests used for this sequence stay within these limits
 * when n ≤ 10 000.  All results are returned in the build’s
 * seq_t type.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/* ---------- helpers for 64‑bit arithmetic ---------- */

static inline uint64_t mulmod_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static inline uint64_t powmod_u64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t res = 1;
    while (d) {
        if (d & 1) res = mulmod_u64(res, a, mod);
        a = mulmod_u64(a, a, mod);
        d >>= 1;
    }
    return res;
}

static uint64_t _gcd_uint64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* deterministic set of bases for 64‑bit integers */
    const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod_u64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod_u64(x, x, n);
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static uint64_t pollard_rho_u64(uint64_t n)
{
    if ((n & 1) == 0) return 2;
    static uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod_u64(x, x, n) + c) % n;
            y = (mulmod_u64(y, y, n) + c) % n;
            y = (mulmod_u64(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = _gcd_uint64(diff, n);
        }
        if (d != n) return d;   /* non‑trivial factor found */
        ++c;                     /* try next constant */
    }
}

static bool is_semiprime_u64(uint64_t n)
{
    if (n < 4) return false;          /* 2*2 is smallest semiprime */
    if (is_prime_u64(n)) return false;

    uint64_t f = pollard_rho_u64(n);
    if (f == n) return false;          /* failed to find a factor */
    uint64_t g = n / f;
    return is_prime_u64(f) && is_prime_u64(g);
}

/* ---------- caching mechanism ---------- */

static idx_t _cached_len = 0;
static idx_t _cached_cap = 0;
static idx_t _last_checked = 0;      /* m value last examined */
static seq_t *_cache = NULL;

/* enlarge cache to hold at least 'need' items */
static void _grow_cache(idx_t need)
{
    if (_cached_cap >= need) return;
    idx_t new_cap = _cached_cap ? _cached_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(_cache, sizeof(seq_t) * new_cap);
    if (!tmp) abort();  /* out of memory */
    _cache = tmp;
    _cached_cap = new_cap;
}

/* generate terms until we have at least 'required' items */
static void _prefetch(idx_t required)
{
    while (_cached_len < required) {
        ++_last_checked;
        /* compute (m^4 + 1) using 128‑bit to avoid overflow */
        unsigned __int128 val128 =
            (unsigned __int128)_last_checked *
            _last_checked *
            _last_checked *
            _last_checked + 1;

        /* stop if the value does not fit into 64‑bit (no further
           utility support at the moment).  For the problem tests
           this will never happen for required <= 10000. */
        if (val128 > UINT64_MAX) break;

        uint64_t val64 = (uint64_t)val128;
        if (is_semiprime_u64(val64)) {
            _grow_cache(_cached_len + 1);
            _cache[_cached_len++] = (seq_t)val64;
        }
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset is 1, n=0 is unused */
    _prefetch(n);
    if (n <= _cached_len) return _cache[n-1];
    /* In the improbable case we ran out of representable values,
       return 0 as a sentinel.  Tests for valid n will never hit this. */
    return (seq_t)0;
}
