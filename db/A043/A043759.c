/*
 * A043759  Numbers n such that number of runs in the base 2 representation of
 *          n is congruent to 5 mod 9.
 * https://oeis.org/A043759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers n such that the number of runs in the base‑2
 *            representation of n is congruent to 5 modulo 9. a(n) returns the
 *            nth such number (a(1)=21)
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 957 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Count the number of runs of 0/1 in the binary representation of x.
   For x > 0 this is the number of maximal consecutive blocks of equal bits.
   If x == 0 this function returns 1 (not used for the sequence). */
static unsigned int binary_runs(unsigned long long x)
{
    if (x == 0ull)
        return 1u;            /* Not reached for the sequence */

    /* Find position of the most significant set bit. */
    int msb = 63;
    while ((x >> msb) == 0u)
        --msb;

    unsigned int run = 1u;                  /* Start with the highest run */
    unsigned int prev = (x >> msb) & 1u;    /* Bit of the MSB */

    for (int pos = msb - 1; pos >= 0; --pos) {
        unsigned int bit = (x >> pos) & 1u;
        if (bit != prev) {
            ++run;
            prev = bit;
        }
    }
    return run;
}

/* Static cache for computed sequence values. */
static seq_t *cache = NULL;
static size_t cache_size = 0;      /* Number of terms already stored */
static size_t cache_capacity = 0;  /* Allocated capacity of cache */
static unsigned long long next_candidate = 1ull;  /* Next integer to test */

/* Ensure the cache has at least `need` elements allocated. */
static void cache_grow(size_t need)
{
    if (cache_capacity >= need)
        return;

    size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    cache = tmp;
    cache_capacity = new_cap;
}

/* Return the nth term of the sequence (a(1) = 21). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* Undefined for n=0, return 0 */

    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        /* Find the next integer whose run count ≡ 5 (mod 9). */
        while (true) {
            unsigned int runs = binary_runs(next_candidate);
            if (runs % 9u == 5u)
                break;
            ++next_candidate;
        }

        /* Store the found number in the cache. */
        cache_grow(cache_size + 1);
        cache[cache_size] = (seq_t)next_candidate;
        ++cache_size;
        ++next_candidate;
    }

    return cache[n - 1];
}
