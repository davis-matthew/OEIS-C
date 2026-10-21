/*
 * A210994  Numbers k such that A000005(k) != 4.
 * https://oeis.org/A210994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer k with A000005(k) != 4 (i.e.
 *            number of divisors of k is not equal to 4)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache for already computed terms */
static seq_t *cached = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* next integer to test */
static uint64_t next_test = 1;

/* ensure the cache has space for at least `need` terms */
static void
ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    cached = (seq_t *)realloc(cached, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* divisor count for an unsigned 64‑bit integer by trial division */
static uint64_t
divisor_count_u64(uint64_t n)
{
    uint64_t d = 1, m = n;
    if ((m & 1ULL) == 0) {
        uint64_t cnt = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++cnt;
        }
        d *= (cnt + 1);
    }
    for (uint64_t p = 3; p <= m / p; p += 2) {   /* p*p <= m */
        if (m % p == 0) {
            uint64_t cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            d *= (cnt + 1);
        }
    }
    if (m > 1)     /* remaining prime factor */
        d *= 2;
    return d;
}

/* fill the cache until it contains at least `n` terms */
static void
fill_cache(idx_t n)
{
    while (cache_len < (size_t)n) {
        uint64_t val = next_test;
        if (divisor_count_u64(val) != 4) {
            cached[cache_len++] = (seq_t)val;
        }
        ++next_test;
    }
}

/* public sequence function */
seq_t
a(idx_t n)
{
    if (n == (idx_t)0)   /* out of range, but we keep definition legal */
        return (seq_t)0;
    ensure_cache_capacity((size_t)n);
    fill_cache(n);
    return cached[(size_t)n - 1];
}
