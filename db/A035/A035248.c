/*
 * A035248  Indices of nonzero terms in expansion of Dirichlet series
 *          Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m=
 *          -7 (A035182).
 * https://oeis.org/A035248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th positive integer whose prime factors congruent
 * to 3, 5 or 6 modulo 7 appear to an even exponent.  Equivalently, let
 * S = {p prime : p ≡ 3,5,6 (mod 7)}.  Then a(n) is the n‑th integer  that
 * contains no element of S to an odd power.  The sequence begins
 * 1,2,4,7,8,9,11,14,16,18,22,23,25,28,…    */

/* The algorithm precomputes a sieve of smallest prime factors (spf) up
 * to a fixed limit (2 000 000).  Then it scans successive integers
 * checking the exponent parity for all primes p ≡ 3,5,6 (mod 7); the
 * test uses the spf array for fast factorisation.  A static cache
 * holds the sequence values so that subsequent calls for larger n
 * reuse previous work.  This runs in O(n log n) time for n ≤ 10 000
 * and uses < 10 MB of memory. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* maximum value to sieve; larger values can be supported by increasing
 * the constant, but 2 000 000 suffices for n up to at least 10 000. */
#define MAX_SIEVE 2000000u

static idx_t   cache_size = 0;     /* number of terms stored in cache */
static seq_t * cache = NULL;       /* dynamic array of sequence terms */
static idx_t   cache_cap  = 0;     /* current capacity of cache array */

static uint32_t * spf = NULL;      /* smallest prime factor array */
static idx_t   spf_limit = 0;      /* limit up to which spf is valid */

static void build_spf(idx_t limit)
{
    spf = (uint32_t *)malloc((limit + 1u) * sizeof(uint32_t));
    if (!spf) return;
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0u;
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == 0u) {
            spf[i] = (uint32_t)i;
            if (i * i <= limit)
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0u)
                        spf[j] = (uint32_t)i;
        }
    spf_limit = limit;
}

static bool allowed_uint64(idx_t x)
{
    while (x > 1) {
        uint32_t p = spf[x];
        if (p == 0u)    /* should not happen for x <= spf_limit */
            p = (uint32_t)x;
        /* count exponent of p */
        idx_t e = 0;
        do {
            x /= p;
            ++e;
        } while (x % p == 0);

        /* check residue class */
        uint32_t r = p % 7u;
        if (r == 3u || r == 5u || r == 6u) {
            if (e & 1u)   /* odd exponent of a forbidden prime */
                return false;
        }
    }
    return true;
}

static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;      /* already have enough terms */

    if (!spf) build_spf(MAX_SIEVE);
    if (!spf) return;                 /* sieve build failed */

    if (cache_cap < n) {
        cache_cap = n > 16 ? n : 16;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return;             /* allocation failed */
        cache = tmp;
    }

    idx_t candidate = cache_size ? (idx_t)cache[cache_size - 1] + 1 : 1;
    idx_t last_scanned = candidate - 1;

    while (cache_size < n && candidate <= (idx_t)MAX_SIEVE) {
        if (allowed_uint64(candidate))
            cache[cache_size++] = (seq_t)candidate;
        ++candidate;
    }
    /* if we exhausted the sieve limit, we could extend the sieve,
       but for n <= 10 000 the above is sufficient. */
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n - 1];
}
