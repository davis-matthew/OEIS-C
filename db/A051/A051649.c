/*
 * A051649  Primes of the form 2310*p + 1 where p is a prime.
 * https://oeis.org/A051649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2310*p + 1 where p is prime
 * Range:     u8 none, u16 n <= 4, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 49 terms through n = 9978 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
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

/* Static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

/* Resize the cache if needed */
static void ensure_cache_capacity(void) {
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
        seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Return the n-th term of OEIS sequence A051649 (1‑based) */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* undefined for offset < 1 */

    if (n <= cache_len) return cache[n-1];    /* Cached term */

    /* Determine starting prime p to search from */
    idx_t p_start = 2;
    if (cache_len > 0) {
        seq_t last_val = cache[cache_len-1];
        seq_t last_p_seq = (last_val - 1) / 2310;
        p_start = ((idx_t)last_p_seq) + 1;
    }

    /* Search for the next terms until n is reached */
    for (idx_t p = p_start; ; ++p) {
        if (!is_prime_seq(p)) continue;       /* Skip non‑primes quickly */

        seq_t candidate = (seq_t)2310 * (seq_t)p + 1;
        if (is_prime_big(candidate)) {           /* Verify 2310*p+1 is prime */
            ensure_cache_capacity();
            cache[cache_len++] = candidate;
            if (cache_len >= n) break;
        }
    }

    return cache[n-1];
}
