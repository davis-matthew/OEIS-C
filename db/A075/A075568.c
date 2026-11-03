/*
 * A075568  Rearrangement of noncomposite numbers such that every partial sum
 *          is composite.
 * https://oeis.org/A075568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rearrangement of noncomposite numbers such that every partial sum is composite.
   Greedy construction: at each step pick the smallest unused noncomposite
   number that keeps the running sum composite. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
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

/* cache for memoized terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t produced = 0;

/* prime data */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static bool *used_prime = NULL;
static bool used_1 = false;

/* running sum */
static seq_t cumulative_sum = 0;

/* ensure list of primes up to an upper bound */
static void ensure_primes(void)
{
    if (prime_list != NULL)
        return;
    idx_t limit = (idx_t)200000;                 /* > prime(10000) */
    build_primes_upto(limit, &prime_list, &prime_count);
    used_prime = (bool*)calloc(prime_count, sizeof(bool));
}

/* grow the cache to hold at least n elements */
static void ensure_cache(size_t n)
{
    if (n <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < n)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

/* the sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n<1, harmless */

    size_t idx = (size_t)n;
    ensure_primes();
    ensure_cache(idx);

    while (produced < idx) {
        seq_t term;
        if (produced == 0) {                 /* first term */
            term = (seq_t)1;
            cumulative_sum = term;
            used_1 = true;
        } else {
            /* find smallest unused prime that keeps the sum composite */
            bool found = false;
            seq_t new_sum = 0;
            for (size_t i = 0; i < prime_count; ++i) {
                if (used_prime[i])
                    continue;
                seq_t p = (seq_t)prime_list[i];
                new_sum = cumulative_sum + p;
                if (!is_prime_big(new_sum)) {    /* composite */
                    term = p;
                    cumulative_sum = new_sum;
                    used_prime[i] = true;
                    found = true;
                    break;
                }
            }
            /* As conjectured, a solution always exists for the tested range.
               If none is found, return 0 (should not happen). */
            if (!found) {
                term = (seq_t)0;
                cumulative_sum = new_sum;      /* keep sum consistent */
            }
        }
        cache[produced++] = term;
    }

    return cache[idx - 1];
}
