/*
 * A201258  Primes of the form 7*n^3 + 10.
 * https://oeis.org/A201258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 68, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   _is_prime64, _mod_mul, _pow_mod, bitlen_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A201258: Primes of the form 7*n^3 + 10.
   a(n) is the n-th such prime (n ≥ 1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/is_prime_big.h"
#include "../utils/_is_prime64.h"
#include "../utils/_mod_mul.h"
#include "../utils/_pow_mod.h"

/* 64‑bit Miller–Rabin primality test ------------------------------------------------- */

static bool _is_prime_seq(seq_t n)
{
    if (bitlen_seq_t(n) <= 64)
        return _is_prime64((uint64_t)n);
    return is_prime_big(n);
}

/* Cache of found primes ------------------------------------------------------------ */

static seq_t *prime_cache = NULL;   /* holds primes of the form 7*k^3+10 */
static size_t   cache_len   = 0;     /* number of cached primes                */
static size_t   cache_cap   = 0;     /* allocated capacity                      */
static idx_t    next_k      = 1;     /* next k to test                         */

static void _ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    prime_cache = tmp;
    cache_cap = new_cap;
}

static void _append_prime(seq_t p)
{
    _ensure_capacity(cache_len + 1);
    prime_cache[cache_len++] = p;
}

static void _generate_up_to(idx_t n)
{
    while (cache_len < n) {
        seq_t k  = (seq_t)next_k;
        seq_t k2 = k * k;
        seq_t k3 = k2 * k;
        seq_t p  = k3 * (seq_t)7 + (seq_t)10;
        if (_is_prime_seq(p)) _append_prime(p);
        ++next_k;
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */
    _generate_up_to(n);
    return prime_cache[n - 1];
}
