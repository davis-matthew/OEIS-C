/*
 * A086208  Prime numbers p with the property that the sequence p-1*2, p-2*3,
 *          p-3*4, p-4*5, ..., consists only of primes and a final 1.
 * https://oeis.org/A086208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: full, pass, 5 terms through n = 4 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for primes p = k(k+1)+1 such that all p - i(i+1) (1 ≤ i < k) are prime. */

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

static seq_t *cache = NULL;     /* cached terms */
static size_t cache_len = 0;    /* number of cached terms */
static size_t cache_cap = 0;    /* capacity of cache array */
static idx_t next_k = 1;        /* next k to test */

/* Ensure that the cache can hold at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (cache_cap < need) {
        size_t newcap = cache_cap ? cache_cap * 2 : 64;
        if (newcap < need) newcap = need;
        seq_t *tmp = mem_resize(seq_t, cache, newcap);
        cache = tmp;
        cache_cap = newcap;
    }
}

/* Append a value to the cache. */
static void add_cache(seq_t val)
{
    ensure_cache(cache_len + 1);
    cache[cache_len++] = val;
}

/* Generate and cache the next term of the sequence. */
static void generate_next(void)
{
    for (;;) {
        idx_t k = next_k++;                     /* candidate k */
        seq_t p = (seq_t)k * ((seq_t)k + 1) + 1; /* p = k(k+1)+1 */

        if (!is_prime_big(p))
            continue;                           /* p must be prime */

        bool ok = true;
        for (idx_t i = 1; i < k; ++i) {
            seq_t sub = (seq_t)i * ((seq_t)i + 1);
            seq_t val = p - sub;
            if (!is_prime_big(val)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            add_cache(p);
            break;                               /* found next term */
        }
    }
}

/* Return the n-th term (a(0)=3). */
seq_t a(idx_t n)
{
    while (cache_len <= (size_t)n)
        generate_next();
    return cache[(size_t)n];
}
