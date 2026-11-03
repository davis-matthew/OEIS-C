/*
 * A138980  a(0)=1; for n>=1, a(n) = smallest prime > a(n-1) such that
 *          (sum{k=0 to n} a(k)) is composite.
 * https://oeis.org/A138980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 111, u32 n <= 111, u64 n <= 111,
 *            u128 n <= 111, big any n
 * Validated: standard, pass, 74 terms through n = 98 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1; for n≥1, a(n) is the smallest prime > a(n‑1) such that
   S(n)=∑_{k=0}^n a(k) is composite.  The sequence is built lazily and
   cached so calls can be made in any order.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

static seq_t *cache = NULL;       /* cache of a(k) values */
static size_t cache_cap = 0;      /* allocated capacity (in elements) */
static size_t cache_len = 0;      /* number of stored elements (last idx+1) */
static seq_t sum_accum = 0;       /* cumulative sum of a(0)..a(last) */

/* Ensure the cache can store indices up to `anew` (inclusive).  */
static void
ensure_cache_capacity(idx_t anew)
{
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        cache[0] = (seq_t)1;            /* a(0) = 1 */
        sum_accum = (seq_t)1;
        cache_len = 1;
    }
    if ((size_t)anew + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)anew + 1)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Extend the sequence up to index `n` (inclusive).  */
static void
extend_to(idx_t n)
{
    if (cache == NULL)
        return;   /* should not happen: ensure_cache_capacity guarantees it */

    size_t last_idx = cache_len - 1;
    if ((idx_t)last_idx >= n)
        return;   /* already computed */

    for (idx_t i = last_idx + 1; i <= n; ++i) {
        seq_t candidate = cache[i - 1] + (seq_t)1;     /* start just above a(i‑1) */
        if (candidate > (seq_t)2 && (candidate & (seq_t)1) == (seq_t)0)
            candidate += (seq_t)1;                    /* skip even numbers >2 */

        for (;;) {
            /* Skip non‑primes.  */
            if (!is_prime_big(candidate)) {
                candidate += (seq_t)1;
                if (candidate > (seq_t)2 && (candidate & (seq_t)1) == (seq_t)0)
                    candidate += (seq_t)1;
                continue;
            }

            seq_t new_sum = sum_accum + candidate;
            if (is_prime_big(new_sum)) {             /* sum must be composite */
                candidate += (seq_t)1;
                if (candidate > (seq_t)2 && (candidate & (seq_t)1) == (seq_t)0)
                    candidate += (seq_t)1;
                continue;
            }

            /* Found the required prime.  */
            cache[i] = candidate;
            sum_accum = new_sum;
            break;
        }
    }
    cache_len = (size_t)n + 1;
}

/* Return a(n) with static caching.  */
seq_t
a(idx_t n)
{
    ensure_cache_capacity(n);
    extend_to(n);
    return cache[(size_t)n];
}
