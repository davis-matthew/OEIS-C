/*
 * A060882  a(n) = n-th primorial (A002110) minus next prime.
 * https://oeis.org/A060882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = product of first n primes (primorial) minus the (n+1)‑th
 *            prime
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 9, i64 n <= 15, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static caches used by all calls */
static uint32_t *primes      = NULL;  /* list of primes found */
static size_t   primes_cap   = 0;     /* capacity of `primes` array    */
static size_t   primes_cnt   = 0;     /* number of primes stored        */
static uint64_t last_checked = 1;    /* last integer checked for primality */

static seq_t *prim_cache = NULL; /* primorials: prim_cache[i] = product of first i primes  */
static size_t prim_cap   = 0;     /* capacity of prim_cache          */
static size_t prim_used  = 0;     /* largest index whose primorial is known  */

/* Ensure we have at least (need+1) primes in 'primes' (indices 0..need). */
static void ensure_primes(size_t need)
{
    while (primes_cnt <= need) {
        uint64_t cand = ++last_checked;
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (primes_cnt >= primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
                if (!tmp) abort();
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = (uint32_t)cand;
        }
    }
}

/* Ensure prim_cache can hold indices up to 'need' and compute missing primorials. */
static void ensure_primorial(size_t need)
{
    /* Grow prim_cache array if necessary */
    if (need >= prim_cap) {
        size_t new_cap = prim_cap ? prim_cap * 2 : 16;
        while (new_cap <= need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(prim_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        prim_cache = tmp;
        prim_cap   = new_cap;
    }

    /* Compute primorials from current value up to 'need' */
    for (size_t i = prim_used + 1; i <= need; ++i) {
        seq_t prod   = prim_cache[i - 1];
        seq_t prime  = (seq_t)primes[i - 1];
        prim_cache[i] = prod * prime;
    }
    prim_used = need;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;          /* convert idx_t to size_t for array access */

    /* Ensure we know enough primes for n (need indices 0..n). */
    ensure_primes(idx);

    /* Initialise prim_cache on first use. */
    if (!prim_cache) {
        prim_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!prim_cache) abort();
        prim_cache[0] = (seq_t)1;    /* P_0 = 1 */
        prim_cap    = 1;
        prim_used   = 0;
    }

    /* Compute primorials up to n if not yet done. */
    if (prim_used < idx)
        ensure_primorial(idx);

    seq_t primorial = prim_cache[idx];
    seq_t next_prime = (seq_t)primes[idx];
    return primorial - next_prime;
}
