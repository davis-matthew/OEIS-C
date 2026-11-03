/*
 * A124129  Primes p for which there are no primes between p and p+sqrt(p).
 * https://oeis.org/A124129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A124129: primes p for which the next prime is greater than p + ⌊√p⌋. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isqrt_seq.h"
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

/* Cached qualifying primes. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t prime_start = 1;   /* last prime processed */

/* Ensure the cache array has room for at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_arr = mem_new(seq_t, new_cap);
    if (!new_arr) return; /* Allocation failure – unlikely in tests. */
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i) new_arr[i] = cache[i];
        mem_free(cache);
    }
    cache     = new_arr;
    cache_cap = new_cap;
}

/* Return the next prime greater than 'start'. */
static seq_t next_prime_from(seq_t start)
{
    seq_t x = start + 1;
    while (!is_prime_big(x)) ++x;
    return x;
}

/* Return the n-th term of A124129 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, return 0 */

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_len) return cache[n - 1];

    /* Generate terms until we have the n‑th. */
    while (cache_len < (size_t)n) {
        /* Next prime after the last processed prime. */
        seq_t p = next_prime_from(prime_start);

        /* The immediate next prime after p. */
        seq_t q = next_prime_from(p);

        /* Check the condition. */
        seq_t s     = isqrt_seq(p);
        seq_t limit = p + s;
        if (q > limit) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = p;
        }

        /* Prepare for the next iteration. */
        prime_start = p;
    }

    return cache[n - 1];
}
