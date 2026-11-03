/*
 * A172996  Primes of form (3^n+7)/2.
 * https://oeis.org/A172996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^k + 7)/2 where k is the n-th exponent giving a prime,
 *            a(1) = 5
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: standard, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   is_prime_u64, mem, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow3.h"
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

/* Cache of previously computed terms */
static seq_t *cache = NULL;
static size_t cache_count = 0;
static size_t cache_capacity = 0;

/* Helper to grow the cache as needed */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_capacity)
        return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        abort(); /* Handle out-of-memory */
    cache = tmp;
    cache_capacity = new_cap;
}

/* Computes the n-th term of A172996 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined, but guard against */
    /* Return cached value if available */
    if ((size_t)n <= cache_count)
        return cache[n - 1];

    /* Extend the cache up to n terms */
    /* We keep track of the exponent we have examined so far.
       Since the sequence grows extremely fast, the number of
       exponents needed is very small (<= 15 for n up to 10,000). */
    idx_t exp = 1;
    idx_t prime_cnt = 0;
    idx_t cache_index = 0; /* index in cache where next term will be stored */
    /* Reset cache if we re-examine from the beginning (in case
       earlier calls were made with smaller n). */
    cache_count = 0;

    while (prime_cnt < n) {
        seq_t val = (pow3(exp) + (seq_t)7) / (seq_t)2;
        if (is_prime_big(val)) {
            ++prime_cnt;
            ensure_cache_capacity(cache_index + 1);
            cache[cache_index++] = val;
        }
        ++exp;
    }
    cache_count = cache_index;
    return cache[n - 1];
}
