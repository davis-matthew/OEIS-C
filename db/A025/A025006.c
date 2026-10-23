/*
 * A025006  a(1) = 7; a(n+1) = a(n)-th nonprime, where nonprimes begin at 1.
 * https://oeis.org/A025006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=7; a(n+1)=the a(n)-th nonprime (nonprimes begin with 1)
 * Range:     u8 n <= 11, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* --- small primes up to 65536 (enough for sqrt of the needed range) --- */
static uint32_t _small_primes[7000];
static size_t   _small_prime_count = 0;
static bool     _small_primes_built = false;

static void _build_small_primes(void)
{
    if (_small_primes_built) return;
    _small_primes_built = true;
    const uint32_t LIMIT = 65536;
    uint8_t sieve[LIMIT + 1];
    memset(sieve, 1, LIMIT + 1);
    sieve[0] = sieve[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (sieve[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                sieve[(size_t)j] = 0;
        }
    }
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (sieve[i])
            _small_primes[_small_prime_count++] = i;
    }
}

/* --- cache for sequence values --- */
static seq_t * _cache = NULL;
static size_t _cache_cap = 0;
static size_t _cache_size = 0;

/* helper to grow the cache */
static void _ensure_cache_capacity(size_t needed)
{
    if (needed <= _cache_cap) return;
    size_t new_cap = _cache_cap ? _cache_cap << 1 : 4;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    _cache = tmp;
    _cache_cap = new_cap;
}

/* --- segmented sieve state --- */
#define SEG_SIZE (1 << 20)          /* 1MiB segment */
static unsigned char *_segment = NULL;  /* composite markers for current segment */
static idx_t         _segment_start = 0; /* inclusive */
static idx_t         _segment_end   = 0; /* exclusive */

/* --- statistics while scanning --- */
static seq_t _cnt_nonprime = 0; /* number of nonprimes seen so far */
static seq_t _next_k = 0;       /* k for which we are looking for the k‑th nonprime */
static idx_t _cur = 1;          /* current integer being examined */

/* load a new segment when needed */
static void _load_segment(void)
{
    if (_cur >= _segment_end) {
        _segment_start = _cur;
        _segment_end   = _segment_start + SEG_SIZE;
        if (!_segment) {
            _segment = (unsigned char *)malloc(SEG_SIZE);
            if (!_segment) abort();
        }
        memset(_segment, 0, SEG_SIZE);
        _build_small_primes();
        for (size_t pi = 0; pi < _small_prime_count; ++pi) {
            uint32_t p = _small_primes[pi];
            uint64_t start = ((uint64_t)(_segment_start + p - 1) / p) * p;
            uint64_t pp = (uint64_t)p * p;
            if (start < pp) start = pp;
            for (uint64_t j = start; j < (uint64_t)_segment_end; j += p)
                _segment[j - _segment_start] = 1;   /* composite */
        }
    }
}

/* initialise the sequence state on first call */
static void _init_sequence(void)
{
    if (_cache_size > 0) return;      /* already initialised */
    _ensure_cache_capacity(1);
    _cache[0] = (seq_t)7;             /* a(1) = 7 */
    _cache_size = 1;
    _next_k = _cache[0];
    _cnt_nonprime = 0;
    _cur = 1;
}

/* --- main sequence routine --- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined offset */
    _init_sequence();

    if (n > _cache_size) {
        /* compute missing terms up to n */
        while (_cache_size < n) {
            _load_segment();
            bool prime = (_cur >= 2 && !_segment[_cur - _segment_start]);

            if (!prime) {               /* nonprime */
                _cnt_nonprime++;
                if (_cnt_nonprime == _next_k) {
                    /* found the next term */
                    _ensure_cache_capacity(_cache_size + 1);
                    _cache[_cache_size] = (seq_t)_cur;
                    _cache_size++;
                    _next_k = _cache[_cache_size - 1];
                }
            }

            _cur++;   /* advance to next integer */
        }
    }

    return _cache[n - 1];
}
