/*
 * A027701  Primes with even number of 1's in binary expansion such that next
 *          prime also has even number of 1's.
 * https://oeis.org/A027701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Validated: standard, pass, 62 terms through n = 864 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate primes p with even popcount and the immediate next prime q
   also having even popcount.  Cache results for repeated calls.  Not a closed form. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Test whether a small seq_t-valued integer is prime.
   Works for all supported seq_t types. */
static bool is_prime_small(seq_t num) {
    if (num < 2) return false;
    for (seq_t d = 2; d * d <= num; ++d) {
        if (num % d == 0) return false;
    }
    return true;
}

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

/* popcount for a seq_t value, works for built‑in or arbitrary‑precision
   types that support &, >> and == 0 */
static seq_t popcount_seq(seq_t x)
{
    seq_t c = 0;
    while (x) {
        c = c + (x & 1);
        x = x >> 1;
    }
    return c;
}

/* prime test dispatching to the appropriate routine */
static bool is_prime_seq(seq_t x)
{
    if (sizeof(seq_t) <= sizeof(idx_t))
        return is_prime_small((idx_t)x);
    else
        return is_prime_big(x);
}

/* find the next prime strictly greater than 'start' (inclusive of start+1) */
static seq_t next_prime_after(seq_t start)
{
    seq_t p = start + 1;
    while (!is_prime_seq(p))
        p = p + 1;
    return p;
}

/* cache storage */
static seq_t *cache          = NULL;
static size_t cache_len      = 0;
static size_t cache_cap      = 0;
static seq_t first_candidate = 3;    /* start with the first odd prime */

/* grow cache to at least min_cap elements */
static void ensure_cache_cap(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < min_cap)
        new_cap = min_cap;
#if defined(__cplusplus)
    cache = reinterpret_cast<seq_t*>(mem_resize(seq_t, cache, new_cap));
#else
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
    if (!cache)
        exit(1);   /* allocation failed */
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* generate terms until we have at least n entries */
    while (cache_len < (size_t)n)
    {
        seq_t cand = first_candidate;

        if (is_prime_seq(cand))
        {
            seq_t pc_cand = popcount_seq(cand);
            if ((pc_cand & 1) == 0) /* even popcount for candidate */
            {
                seq_t nextp = next_prime_after(cand);
                seq_t pc_next = popcount_seq(nextp);
                if ((pc_next & 1) == 0)   /* even popcount for next prime */
                {
                    ensure_cache_cap(cache_len + 1);
                    cache[cache_len++] = cand;
                }
            }
        }

        first_candidate = first_candidate + 2; // skip even numbers
    }

    return cache[(size_t)n - 1];
}
