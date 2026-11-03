/*
 * A051078  Primes p such that x^18 = -2 has a solution mod p.
 * https://oeis.org/A051078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 83 terms through n = 984 (2026-11-03)
 * Helpers:   gcd_u64, is_prime_u64, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search primes p such that the congruence x^18 ≡ -2 (mod p) is solvable.
   Implements A051078 with caching for arbitrary-order calls. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/powmod.h"
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

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* Euclidean algorithm for 64‑bit unsigned integers */

seq_t a(idx_t n)
{
    /* Static cache of primes satisfying the condition. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t last_p = 0;      /* last prime found */

    if (n == 0)                /* safety; offset is 1 */
        return (seq_t)0;

    /* Return cached value if already computed. */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    uint64_t p;
    if (cache_len == 0)
        p = 2;
    else
        p = last_p + 1;               /* start after the last cached prime */

    /* Ensure odd after 2 */
    if (p > 2 && (p & 1) == 0)
        p++;

    /* Generate primes until we have enough. */
    while (cache_len < (size_t)n) {
        if (is_prime_big((seq_t)p)) {
            bool ok = false;

            if (p == 2) {
                /* 0^18 = -2 (mod 2) */
                ok = true;
            } else {
                uint64_t pm1 = p - 1;
                uint64_t d = gcd_u64(pm1, 18ULL);
                uint64_t exp = pm1 / d;

                seq_t base = (seq_t)p - 2;       /* (-2) mod p */
                seq_t r = powmod(base, exp, (seq_t)p);

                if (r == (seq_t)1)
                    ok = true;
            }

            if (ok) {
                /* Append to cache */
                grow_seq_array(&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = (seq_t)p;
                last_p = p;
            }
        }

        /* Advance to the next candidate (skip evens). */
        if (p == 2)
            p = 3;
        else
            p += 2;
    }

    return cache[n - 1];
}
