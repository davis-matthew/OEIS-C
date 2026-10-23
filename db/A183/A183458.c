/*
 * A183458  Number of (n+4)X5 binary arrays with every 1 having exactly three
 *          king-move neighbors equal to 1 but with no 2X2 blocks of 1s.
 * https://oeis.org/A183458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence from OEIS A183458
   a(n) = a(n-1) + 43*a(n-3) - 41*a(n-4) - 613*a(n-6) + 571*a(n-7)
          - 15*a(n-8) + 3083*a(n-9) - 2902*a(n-10) + 333*a(n-11)
          - 2977*a(n-12) + 2546*a(n-13) - 1433*a(n-14) + 405*a(n-15)
          + 1215*a(n-16) - 137*a(n-17) - 12*a(n-18) - 168*a(n-19)
          - 84*a(n-20)
   with initial values a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=4,
   a(6)=21, a(7)=40, a(8)=60, a(9)=344, a(10)=706,
   a(11)=1115, a(12)=5961, a(13)=12955, a(14)=21430,
   a(15)=105338, a(16)=240405, a(17)=415187, a(18)=1882817,
   a(19)=4477153, a(20)=8046028 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#define BASE_COUNT 20

/* Raw values for the first 20 terms (index 0 unused). */
static const seq_t base_values[BASE_COUNT + 1] = {
    0,          /* dummy 0‑index */
    1, 1, 2, 3, 4,
    21, 40, 60, 344, 706,
    1115, 5961, 12955, 21430,
    105338, 240405, 415187,
    1882817, 4477153, 8046028
};

/* Cache for previously computed terms.  Index 0 is unused. */
static seq_t *cache   = NULL;  /* pointer to start of array */
static idx_t  cache_size = 0;  /* highest index that has been filled */
static size_t cache_cap   = 0;  /* allocated array size (elements) */

/* Allocate or grow the cache to hold indices 0 … required (inclusive). */
static void ensure_cache_capacity(size_t required)
{
    if (cache_cap > required)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= required)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
    if (tmp)
        cache = tmp;
    cache_cap = new_cap;
}

/* Fill the cache with the known base values if they are not already present. */
static void initialize_base(void)
{
    if (cache_size >= BASE_COUNT)
        return;                /* already initialized */

    ensure_cache_capacity(BASE_COUNT);

    for (idx_t i = 1; i <= BASE_COUNT; ++i)
        cache[i] = base_values[i];
    cache_size = BASE_COUNT;
}

/* Public routine: a(n) for n >= 1.  May be called in any order. */
seq_t a(idx_t n)
{
    if (n == 0)                /* safety for 0‑indexed calls */
        return (seq_t)0;

    if (n <= BASE_COUNT)       /* retrieve from table directly */
        return base_values[(size_t)n];

    /* Ensure that base values are present in the cache. */
    initialize_base();

    /* If we need higher values, extend the cache. */
    if (cache_size < n) {
        /* Allocate space for all indices up to n. */
        ensure_cache_capacity((size_t)n);

        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t val = cache[i - 1];
            val += (seq_t)43   * cache[i - 3];
            val += (seq_t)571  * cache[i - 7];
            val += (seq_t)3083 * cache[i - 9];
            val += (seq_t)333  * cache[i - 11];
            val += (seq_t)2546 * cache[i - 13];
            val += (seq_t)405  * cache[i - 15];
            val += (seq_t)1215 * cache[i - 16];
            val -= (seq_t)41   * cache[i - 4];
            val -= (seq_t)613  * cache[i - 6];
            val -= (seq_t)15   * cache[i - 8];
            val -= (seq_t)2902 * cache[i - 10];
            val -= (seq_t)2977 * cache[i - 12];
            val -= (seq_t)1433 * cache[i - 14];
            val -= (seq_t)137  * cache[i - 17];
            val -= (seq_t)12   * cache[i - 18];
            val -= (seq_t)168  * cache[i - 19];
            val -= (seq_t)84   * cache[i - 20];
            cache[i] = val;
        }
        cache_size = n;
    }

    return cache[(size_t)n];
}
