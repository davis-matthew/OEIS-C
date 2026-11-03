/*
 * A168025  Noncomposite numbers in the western ray of the Ulam spiral as
 *          oriented on the March 1964 cover of Scientific American.
 * https://oeis.org/A168025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*k^2 − 7*k + 4, where k is the (n+1)-th positive integer
 *            for which 4*k^2 − 7*k + 4 is 1 or a prime. The sequence is
 *            generated in order of increasing k
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 44 terms through n = 940 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_len = 0;     /* number of values stored */
static size_t cache_cap = 0;     /* capacity of cache */
static idx_t next_k = 1;         /* next integer k to test */

/* Ensure cache can hold at least "need" elements. */
static void grow_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
    while (new_cap < need) new_cap = new_cap * 2 + 8;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term (0‑based) of the sequence. */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n+1 of them. */
    while (cache_len <= (size_t)n) {
        seq_t k = (seq_t)next_k;
        /* Compute 4*k^2 − 7*k + 4.  All operations are performed in
           seq_t; the sequence values fit into seq_t for all n up
           to the limits required by this problem. */
        seq_t val = k * ((k * (seq_t)4) - (seq_t)7) + (seq_t)4;

        /* Keep the value if it is 1 or a prime. */
        if (val == (seq_t)1 || is_prime_big(val)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = val;
        }
        ++next_k;
    }
    return cache[(size_t)n];
}
