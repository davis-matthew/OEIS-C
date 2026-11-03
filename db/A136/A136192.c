/*
 * A136192  Primes p such that 2p-3 and 2p+3 are both prime (A092110), with
 *          last decimal of p being 7.
 * https://oeis.org/A136192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 116, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 987 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A136192: primes p such that 2p‑3 and 2p+3 are both prime
 * and the decimal representation of p ends in 7.  The sequence is
 * evaluated on demand and memoised in a static cache so that calls
 * need not be in increasing order.  The implementation uses the helper
 * function is_prime_big to test primality for arbitrary‑precision
 * seq_t values.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
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

/* Static cache storing discovered terms of the sequence. */
static seq_t *cache     = NULL;   /* array of terms */
static size_t cache_sz  = 0;      /* number of terms currently stored */
static size_t cache_cap = 0;      /* capacity of the cache */

/* Ensure the cache can hold at least 'need' terms. */
static void
ensure_cache_size(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        abort();                 /* allocation failure – should not happen */
    cache     = tmp;
    cache_cap = new_cap;
}

/* Main sequence function.  An O(k) scan is performed each time a new
 * term is required, but results are cached for future calls. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n < 1; guard value */

    /* Return cached value if already computed. */
    if (cache_sz >= (size_t)n)
        return cache[(size_t)n - 1];

    /* Generate terms until we have at least n of them. */
    seq_t candidate = 7;          /* first prime ending in 7 */

    /* If we had previously generated some terms, resume from the last
     * candidate that was tried rather than starting at 7 again. */
    if (cache_sz > 0) {
        /* The last accepted term is cache[cache_sz-1].
         * The next candidate should be this + 10. */
        candidate = cache[cache_sz - 1];
        candidate = candidate + 10;
    }

    while (cache_sz < (size_t)n) {
        /* Only numbers ending in 7 are relevant.  Test primality of the
         * candidate itself and of 2*p-3 and 2*p+3. */
        if (is_prime_big(candidate)) {
            seq_t q1 = 2 * candidate - 3;
            seq_t q2 = 2 * candidate + 3;
            if (is_prime_big(q1) && is_prime_big(q2)) {
                /* Store the new term. */
                ensure_cache_size(cache_sz + 1);
                cache[cache_sz++] = candidate;
            }
        }
        candidate = candidate + 10;          /* next number ending with 7 */
    }

    return cache[(size_t)n - 1];
}
