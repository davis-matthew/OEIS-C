/*
 * A156323  List of prime pairs of the form (p, p+12).
 * https://oeis.org/A156323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the primes p and p+12 alternately, where both p * and
 *            p+12 are prime. The sequence starts 5, 17, 7, 19, 11, 23, …
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 1000 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds the sequence lazily, storing the
 * computed terms in a resizable cache.  It uses the helper
 * is_prime_big (provided in ../utils/is_prime_big.h) for
 * prime testing; this routine works for all supported types of
 * seq_t, including arbitrary‑precision big integers.
 */

#include <stdlib.h>
#include <stdbool.h>
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

static seq_t *cache    = NULL;   /* array of terms already computed   */
static size_t cache_len = 0;     /* number of cached terms            */
static size_t cache_cap = 0;     /* current allocated capacity        */

static seq_t next_candidate = 5; /* first odd number to examine       */

/* Resize the cache so that it can hold at least `need` more elements. */
static void grow_cache(size_t need)
{
    size_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 64;
    while (new_cap < cache_len + need) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();
    cache   = tmp;
    cache_cap = new_cap;
}

/* Append a value to the cache, expanding it if necessary. */
static void cache_append(seq_t v)
{
    grow_cache(1);
    cache[cache_len++] = v;
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) abort();     /* sequence is 1‑indexed */

    /* Generate terms until we have at least *n* cached. */
    while (cache_len < (size_t)n) {
        if (is_prime_big(next_candidate)) {
            seq_t p12 = next_candidate + (seq_t)12;
            if (is_prime_big(p12)) {
                cache_append(next_candidate); /* p   */
                cache_append(p12);             /* p+12 */
            }
        }
        next_candidate += (seq_t)2;          /* next odd number */
    }

    return cache[(size_t)n - 1];
}
