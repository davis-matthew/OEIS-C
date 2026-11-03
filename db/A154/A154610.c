/*
 * A154610  Numbers n such that 13n + 5 is prime.
 * https://oeis.org/A154610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 572, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 948 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that 13*n + 5 is prime (OEIS A154610) */

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

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* Treat index 0 as out of range (not used by the OEIS interface). */
    if (n == 0)
        return 0;

    /* Static cache of already generated terms. */
    static seq_t *cache      = NULL;   /* array of terms */
    static size_t cache_len  = 0;      /* how many terms are stored */
    static size_t cache_cap  = 0;      /* current capacity of the array */
    static idx_t  next_candidate = 0;  /* next n to test */

    /* Return the value if we already have it. */
    if (cache_len >= n)
        return cache[n - 1];

    /* Grow the cache until it contains the requested term. */
    while (cache_len < n) {
        /* Search for the next n such that 13*n+5 is prime. */
        while (true) {
            seq_t val = (seq_t)13 * (seq_t)next_candidate + (seq_t)5;
            if (is_prime_big(val))
                break;          /* found a valid n */
            next_candidate++;   /* try the next integer */
        }

        /* Ensure we have space for a new term. */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp)        /* memory allocation failed */
                abort();
            cache    = tmp;
            cache_cap = new_cap;
        }

        /* Store the discovered term. */
        cache[cache_len] = (seq_t)next_candidate;
        cache_len++;

        /* Advance to the next integer for future searches. */
        next_candidate++;
    }

    /* Now cache_len >= n, return the requested element. */
    return cache[n - 1];
}
