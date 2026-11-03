/*
 * A176616  Primes of the form x^2 + 7*y^2, where x and y=x+1 are consecutive
 *          natural numbers.
 * https://oeis.org/A176616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*(k-1)^2 + 14*(k-1) + 7, for k >= 1, and only when the *
 *            value is prime. a(1) = 7, a(2) = 29,
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 993 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
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

/* Ensure we can store at least 'needed' elements in *ptr. */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
    if (new_cap < needed) new_cap = needed;
    *ptr = realloc(*ptr, new_cap * elem_size);
    *cap = new_cap;
}

/* Cache of found primes of the required form. */
static seq_t *prime_cache = NULL;
static size_t cache_cap = 0;
static size_t prime_count = 0;
static idx_t next_x = 0;  /* Next x to test (x = n-1). */

seq_t a(idx_t n)
{
    /* n >= 1 by contract. */
    while (prime_count < n) {
        /* Compute 8*x^2 + 14*x + 7, with x = next_x. */
        seq_t xsq = (seq_t)next_x * (seq_t)next_x;
        seq_t val = (seq_t)8 * xsq + (seq_t)14 * (seq_t)next_x + (seq_t)7;

        if (is_prime_big(val)) {
            if (prime_count == cache_cap)
                ensure_capacity(cache_cap > 0 ? cache_cap << 1 : 16,
                               &cache_cap, (void **)&prime_cache, sizeof(seq_t));
            prime_cache[prime_count++] = val;
        }
        ++next_x;
    }
    return prime_cache[n - 1];
}
