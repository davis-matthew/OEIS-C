/*
 * A067201  Numbers k such that k^2 + 2 is prime.
 * https://oeis.org/A067201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 981 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth integer k ≥ 0 such that k² + 2 is prime.
   The function computes terms on demand and caches them. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

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

static seq_t *cache = NULL;           /* cached k values */
static size_t cache_size = 0;         /* number of values stored */
static size_t cache_capacity = 0;     /* allocated capacity */
static idx_t last_k = 0;              /* next k to test after last_k-1 */

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_capacity) return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_capacity = new_cap;
}

/* Generate terms until we have at least 'target' entries in cache. */
static void generate_up_to(size_t target)
{
    while (cache_size < target)
    {
        idx_t k = last_k;

        /* Compute k² + 2 in 128‑bit arithmetic. */
        __uint128_t sum128 = (__uint128_t)k * k + 2;

        /* For small seq_t types, stop when sum would overflow seq_t. */
        if (sizeof(seq_t) <= 8)
        {
            __uint128_t max_val;
            if (sizeof(seq_t) == 8)
                max_val = (__uint128_t)UINT64_MAX;
            else
                max_val = ((__uint128_t)1 << (8 * sizeof(seq_t))) - 1;

            if (sum128 > max_val) break;   /* cannot represent further values */
        }

        /* Test primality. */
        bool prime;
        if (sizeof(seq_t) <= 8)
        {
            prime = is_prime_u64((uint64_t)sum128);
        }
        else
        {
            prime = is_prime_big((seq_t)sum128);
        }

        if (prime)
        {
            ensure_capacity(cache_size + 1);
            cache[cache_size++] = (seq_t)k;
        }

        last_k++;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* no definition below offset */

    generate_up_to((size_t)n);
    if (n > cache_size) return 0;   /* sequence cannot be represented */
    return cache[n - 1];
}
