/*
 * A179131  Numerators of A178381(4*n+1)/A178381(4*n).
 * https://oeis.org/A179131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A179131 – Numerators of A178381(4*n+1)/A178381(4*n)
 * Recurrence (for n > 6):
 *     a(n) = 18*a(n-3) - a(n-6)
 * Initial values:
 *     a(0) = 1, a(1) = 5, a(2) = 25, a(3) = 65, a(4) = 85,
 *     a(5) = 445, a(6) = 1165
 * This implementation memoises the sequence in a static cache.  It works
 * for any integer type seq_t (signed/unsigned 8/16/32/64 or arbitrary‑
 * precision) as long as seq_t supports the operators +, -, * and the
 * literal initialisation shown below (valid in both C11 and C++17). */

#include <stddef.h>

/* Largest pre‑allocated index that the static cache covers. */
#define CACHE_MAX 20001

/* Pre‑computed constant 18 in the type seq_t. */
static const seq_t C18 = 18;

/* Base values for n = 0 … 6. */
static const seq_t base_vals[7] = {
    1,   /* a(0) */
    5,   /* a(1) */
    25,  /* a(2) */
    65,  /* a(3) */
    85,  /* a(4) */
    445, /* a(5) */
    1165 /* a(6) */
};

/* Static cache and state. */
static seq_t cache[CACHE_MAX];
static size_t cache_len = 0;        /* number of values currently stored */

/* Initialise the cache with the initial values. */
static void init_cache(void)
{
    if (cache_len != 0)   /* already initialised */
        return;
    for (size_t i = 0; i < 7; ++i)
        cache[i] = base_vals[i];
    cache_len = 7;
}

/* The main sequence function. */
seq_t a(idx_t n)
{
    /* Lazily initialise the cache. */
    init_cache();

    /* If the requested index is already cached, return it immediately. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Extend the cache up to (min(n, CACHE_MAX-1)). */
    size_t target = (size_t)n + 1;
    if (target > CACHE_MAX)
        target = CACHE_MAX;

    for (size_t i = cache_len; i < target; ++i) {
        /* a(i) = 18*a(i-3) - a(i-6)  for i > 6 */
        cache[i] = C18 * cache[i - 3] - cache[i - 6];
    }
    cache_len = target;

    if ((size_t)n < cache_len)   /* return value if now cached */
        return cache[(size_t)n];

    /* If n exceeds the static cache, compute on the fly with a 7‑slot buffer. */
    seq_t buf[7];
    /* Load the last 7 known values into the buffer. */
    size_t start = (cache_len >= 7) ? cache_len - 7 : 0;
    for (size_t i = 0; i < 7; ++i)
        buf[i] = cache[start + i];

    /* Compute successive terms up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        buf[i % 7] = C18 * buf[(i - 3) % 7] - buf[(i - 6) % 7];
    }
    return buf[(size_t)n % 7];
}
