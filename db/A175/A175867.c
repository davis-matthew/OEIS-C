/*
 * A175867  a(0) = 2; a(n) = a(n - 1) * 2 + 1 if a(n - 1) is prime, or a(n -
 *          1) / (smallest prime factor) if it is composite.
 * https://oeis.org/A175867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=2; a(n)=a(n-1)*2+1 if a(n-1) is prime; otherwise
 *            a(n)=a(n-1)/(smallest prime factor of a(n-1))
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 77 (2026-11-03)
 * Helpers:   is_prime_u64, mem, smallest_prime_factor_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/smallest_prime_factor_seq.h"
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

static seq_t *cache = NULL;      /* computed terms */
static idx_t cache_size = 0;     /* number of terms stored */
static size_t cache_cap  = 0;    /* allocated capacity */

/* Extend cache up to at least max_n (inclusive). */
static void ensure_cache(idx_t max_n)
{
    if (max_n < cache_size)
        return;

    size_t new_cap = (size_t)(max_n + 1);

    if (cache == NULL) {
        cache = mem_new(seq_t, new_cap);
        if (!cache) abort();
        cache_cap = new_cap;
        cache[0] = (seq_t)2;
        cache_size = 1;
    } else if (new_cap > cache_cap) {
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = cache_size; i <= max_n; ++i) {
        seq_t prev = cache[i - 1];
        if (is_prime_big(prev)) {
            cache[i] = prev * (seq_t)2 + (seq_t)1;
        } else {
            seq_t spf = smallest_prime_factor_seq(prev);
            cache[i] = prev / spf;
        }
    }
    cache_size = (idx_t)(max_n + 1);
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
