/*
 * A056709  Naught-y primes, primes with noughts (or zeros).
 * https://oeis.org/A056709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1689, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9844 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: This function searches for primes that contain at least
 * one decimal zero digit (the A056709 “naught‑y primes”).
 */

#include <stdbool.h>
#include <stdlib.h>
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

/* Forward declaration of has_zero_digit */
static inline bool has_zero_digit(seq_t n)
{
    /* Zero digit check: iterate over decimal digits */
    while (n > 0) {
        if ((n % 10) == 0)
            return true;
        n /= 10;
    }
    return false;
}

/* Static cache for computed terms */
static seq_t  *cache      = NULL;        /* array of seq_t values */
static idx_t   cached     = 0;           /* number of stored terms   */
static size_t  capacity   = 0;           /* current capacity of cache */
static seq_t  last_checked = 0;          /* highest integer examined so far */

/* Ensure cache holds at least `n` elements (resize if necessary) */
static void ensure_cache(idx_t n)
{
    if (n <= cached)
        return;
    if (capacity == 0)
        capacity = 4;
    while (capacity < n)
        capacity <<= 1;                 /* double until large enough */
    seq_t *tmp = mem_resize(seq_t, cache, capacity);
    if (!tmp)
        abort();                        /* out of memory */
    cache = tmp;
}

/* Sequence accessor: a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined offset – graceful */

    ensure_cache(n);

    /* Generate candidates until we have enough terms */
    seq_t candidate = last_checked + 1;
    while (cached < n) {
        if (is_prime_big(candidate) && has_zero_digit(candidate))
            cache[cached++] = candidate;
        ++candidate;
    }
    last_checked = candidate - 1;
    return cache[n - 1];
}
