/*
 * A057737  Primes of the form 2^p + 3, where p is prime.
 * https://oeis.org/A057737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^p + 3, where p is the nth prime
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   ipow_u64_idx, is_prime_seq, is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/is_prime_seq.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/*--- Cached terms -------------------------------------------------------*/
static seq_t *cache = NULL;      /* array holding found terms           */
static size_t cache_size = 0;    /* number of terms currently stored    */
static size_t cache_cap  = 0;    /* current allocation size             */

/*--- Progress marker ----------------------------------------------------*/
/* The last prime examined by the generator. */
static uint64_t last_checked_prime = 1;

/*--- Exponent limit for fixed‑width seq_t ------------------------------*/
static bool limit_inited = false;
static unsigned max_exponent_limit = 0;

/*--- Ensure cache capacity ---------------------------------------------*/
static inline void ensure_cache_capacity(size_t needed)
{
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        if (new_cap < needed) new_cap = needed;
        cache = mem_resize(seq_t, cache, new_cap);
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Undefined for n == 0 – simply return 0. */
    if (n == 0) return (seq_t)0;

    /* Initialise exponent limit on first call. */
    if (!limit_inited) {
        limit_inited = true;
        /* For fixed‑width unsigned types the limit is (bits - 1). */
        if (sizeof(seq_t) <= 8) {
            /* Unsigned 64‑bit: 2^63+3 < UINT64_MAX → max exp = 63. */
            max_exponent_limit = (sizeof(seq_t) * 8) - 1;
        } else {
            /* Larger unsigned types (e.g. __int128) or arbitrary precision
               objects – no practical limit needed. Use the largest
               possible exponent. */
            max_exponent_limit = (unsigned)-1;
        }
    }

    /* Extend the cached sequence until it contains at least n terms. */
    while (cache_size < n) {
        /* Get the next prime p > last_checked_prime. */
        uint64_t p = next_prime(last_checked_prime);
        last_checked_prime = p;

        /* If seq_t is a fixed‑width type, stop when the exponent would
           overflow. */
        if (max_exponent_limit != (unsigned)-1 && p > max_exponent_limit)
            break;

        /* Compute 2^p + 3. */
        seq_t val = ipow_u64_idx(2, p);
        val += (seq_t)3;

        /* Keep the term only if it is prime. */
        if (is_prime_big(val)) {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = val;
            if (cache_size >= n)
                return cache[n - 1];
        }
    }

    /* If we did not gather enough terms, the requested index is out of
       range for this build. Return 0. */
    return (seq_t)0;
}
