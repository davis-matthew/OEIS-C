/*
 * A087379  Beginning with 2, primes such that the difference between two
 *          successive terms is a distinct composite number.
 * https://oeis.org/A087379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   _mul_mod64, _pow_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A087379: primes with distinct composite differences.
 * a(0) = 2. For n ≥ 1, a(n) is the next prime such that
 *   d = a(n) - a(n-1) is a composite number not used before.
 * The set of used differences starts empty.
 * The implementation keeps mutable static state so that each call
 * may be in arbitrary order and still returns the correct term.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/_mul_mod64.h"
#include "../utils/_pow_mod64.h"

/* ------------------------------------------------------------ */
/* small prime table for quick divisibility checks              */
static const uint64_t _small_primes[] =
    {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
     23ULL, 29ULL, 31ULL, 37ULL};

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* deterministic Miller–Rabin for 64‑bit integers                */
static bool _is_prime64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < sizeof(_small_primes)/sizeof(_small_primes[0]); ++i) {
        uint64_t p = _small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* bases that make the test deterministic for 64‑bit values */
    static const uint64_t _bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(_bases)/sizeof(_bases[0]); ++i) {
        uint64_t a = _bases[i] % n;
        if (a == 0) continue;
        uint64_t x = _pow_mod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = _mul_mod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/* dynamic bitset for used composite differences               */
static uint8_t *_used_bits = NULL;   /* bit array (least significant bit of byte 0 = diff 0) */
static size_t   _used_bits_elems = 0;/* number of bytes allocated */

/* ensure the bitset can hold bit 'idx' */
static void _ensure_used(uint64_t idx)
{
    uint64_t needed_bits = idx + 1;
    size_t   needed_bytes = (needed_bits + 7) / 8;

    if (needed_bytes <= _used_bits_elems) return;

    size_t new_bytes = _used_bits_elems ? _used_bits_elems * 2 : 8;
    if (new_bytes < needed_bytes) new_bytes = needed_bytes;

    uint8_t *new_arr = (uint8_t *)realloc(_used_bits, new_bytes);
    if (!new_arr) exit(1);
    memset(new_arr + _used_bits_elems, 0, new_bytes - _used_bits_elems);
    _used_bits = new_arr;
    _used_bits_elems = new_bytes;
}

/* query a bit */
static bool _used_get(uint64_t idx)
{
    if (idx >= (uint64_t)_used_bits_elems * 8) return false;
    return (_used_bits[idx >> 3] >> (idx & 7)) & 1;
}

/* set a bit */
static void _used_set(uint64_t idx)
{
    _ensure_used(idx);
    _used_bits[idx >> 3] |= (1U << (idx & 7));
}

/* ------------------------------------------------------------ */
/* the required sequence function                               */
seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0) return (seq_t)2;

    /* static state across calls */
    static uint64_t _last_prime = 2;   /* a(0) */
    static idx_t    _generated  = 1;   /* number of terms already produced (includes a(0)) */

    /* generate until we have at least n+1 terms */
    while (_generated <= n) {
        /* search for next admissible prime */
        uint64_t cand = _last_prime + 1;
        if (cand <= 2) cand = 2;
        if (cand % 2 == 0) cand++;           /* start with odd */
        for (;;) {
            if (_is_prime64(cand)) {
                uint64_t diff = cand - _last_prime;
                if (diff > 1 && !_is_prime64(diff) && !_used_get(diff)) {
                    /* acceptable prime */
                    _used_set(diff);
                    _last_prime = cand;
                    ++_generated;
                    break;
                }
            }
            cand += 2;  /* skip even numbers */
        }
    }
    return (seq_t)_last_prime;
}
