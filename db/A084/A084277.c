/*
 * A084277  Pseudo-random numbers: Davenport's generator for 32-bit integers.
 * https://oeis.org/A084277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((b(n+1) >> 16) & 0x7FFF) where b(0) = 1 and b(n+1) =
 *            b(n) * 2147001325 + 715136305
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const uint32_t MULT   = 2147001325U; /* 2147001325 */
static const uint32_t ADD    = 715136305U;  /* 715136305 */
static const uint32_t MASK   = 0x7FFFFFFFU; /* keep 31 bits (bits 0–30) */

static seq_t *cache = NULL;          /* cached a(n) values       */
static size_t cap   = 0;             /* current capacity of cache*/
static idx_t  max_n = (idx_t)-1;      /* highest n that has been cached */
static uint32_t b_cur = 0;           /* b(current n)  (for next step)*/
static bool    initialized = false;  /* flag to ensure one-time init */

static void ensure_capacity(size_t needed)
{
    if (needed <= cap) return;
    size_t new_cap = cap ? cap << 1 : 16;
    if (new_cap < needed) new_cap = needed;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cap   = new_cap;
}

static void initialize(void)
{
    if (initialized) return;
    initialized = true;
    cache = NULL;
    cap   = 0;
    b_cur = 1U;                      /* b(0) */
}

/* Code supplied to the build system. */
seq_t a(idx_t n)
{
    initialize();

    /* On first call we compute a(0) from b(1). */
    if (max_n == (idx_t)-1)
    {
        uint64_t prod = (uint64_t)b_cur * MULT;
        prod += ADD;
        b_cur = (uint32_t)(prod & MASK);          /* b(1) */
        ensure_capacity(1);
        cache[0] = (seq_t)((b_cur >> 16) & 0x7FFF);
        max_n = 0;
    }

    if (n <= max_n) return cache[(size_t)n];

    ensure_capacity((size_t)n + 1);

    for (idx_t i = max_n + 1; i <= n; ++i)
    {
        uint64_t prod = (uint64_t)b_cur * MULT;
        prod += ADD;
        b_cur = (uint32_t)(prod & MASK);          /* now b(i+1) */
        cache[(size_t)i] = (seq_t)((b_cur >> 16) & 0x7FFF);
    }
    max_n = n;
    return cache[(size_t)n];
}
