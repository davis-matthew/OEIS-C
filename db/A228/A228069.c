/*
 * A228069  Signed pseudo characteristic function of primes by annihilation of
 *          composites up to p-1, here p=13 and sign (+).
 * https://oeis.org/A228069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rule: a(n) is the nth positive integer n such that
   n is not divisible by 2,3,5,7,11 and
   (⌊n/2⌋ + ⌊n/3⌋ + ⌊n/5⌋ + ⌊n/7⌋ + ⌊n/11⌋) is even.
   This is equivalent to saying the product
   sin(n·π/2)·sin(n·π/3)·sin(n·π/5)·sin(n·π/7)·sin(n·π/11) has sign +1. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Primes up to 11 */
static const unsigned l_sizes[] = { 2, 3, 5, 7, 11 };
static const size_t l_count = sizeof(l_sizes) / sizeof(l_sizes[0]);

/* Cache limit – enough for all tests (n ≤ 10000). */
#define CACHE_MAX 65536
static uint64_t cache_u64[CACHE_MAX];
static size_t cacheCount = 0;      /* number of terms stored in cache */
static idx_t lastNumber = 0;     /* last integer that was inspected */

/* Fill the cache until it contains CACHE_MAX terms. */
static void fill_cache(void)
{
    while (cacheCount < CACHE_MAX) {
        lastNumber++;                 /* next integer candidate */

        /* Skip numbers divisible by any of the primes. */
        bool bad = false;
        for (size_t i = 0; i < l_count; ++i) {
            if ((lastNumber % l_sizes[i]) == 0) { bad = true; break; }
        }
        if (bad) continue;

        /* Check parity of the sum of floor divisions. */
        unsigned parity = 0;
        for (size_t i = 0; i < l_count; ++i)
            parity ^= (unsigned)((lastNumber / l_sizes[i]) & 1ULL);

        if (parity != 0) continue;           /* product negative */

        /* Store this term. */
        cache_u64[cacheCount++] = (uint64_t)lastNumber;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 */

    /* Ensure the cache has at least n terms. */
    if (cacheCount < (size_t)n) {
        if (cacheCount < CACHE_MAX) {
            fill_cache();                  /* fill to maximum capacity */
        }
    }

    if (cacheCount >= (size_t)n) {
        return (seq_t)cache_u64[n - 1];     /* previously cached */
    }

    /* n exceeds CACHE_MAX – generate the remaining terms on the fly. */
    idx_t found = (idx_t)cacheCount;
    idx_t current = lastNumber + 1;

    while (found < n) {
        /* divisibility test */
        bool bad = false;
        for (size_t i = 0; i < l_count; ++i) {
            if ((current % l_sizes[i]) == 0) { bad = true; break; }
        }
        if (!bad) {
            unsigned parity = 0;
            for (size_t i = 0; i < l_count; ++i)
                parity ^= (unsigned)((current / l_sizes[i]) & 1ULL);
            if (parity == 0) {
                ++found;
                if (found == n) return (seq_t)current;
            }
        }
        ++current;
    }

    /* Should never reach here. */
    return (seq_t)0;
}
