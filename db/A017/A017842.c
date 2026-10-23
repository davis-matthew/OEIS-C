/*
 * A017842  Expansion of 1/(1-x^5-x^6-x^7-x^8-x^9-x^10-x^11).
 * https://oeis.org/A017842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 50, u32 n <= 92, u64 n <= 178,
 *            u128 n <= 349, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A017842: Number of compositions of n into parts 5..11.
 *
 * Formula: a(n) = a(n-5) + a(n-6) + a(n-7) + a(n-8) + a(n-9) + a(n-10) + a(n-11)
 * Initial values: a(0)=1, a(1)=a(2)=a(3)=a(4)=0, a(5)=a(6)=a(7)=a(8)=a(9)=1,
 * a(10)=2, a(11)=3.
 *
 * The implementation uses a static cache array and fills values on demand.
 * It is valid both in C11 and C++17.  The array size is chosen to be
 * comfortably larger than the largest n used in the tests (≈10000).
 */

#include <stdlib.h>   /* abort */

#define CACHE_CAP 200000   /* must be > maximum n expected (≈10000) */

static seq_t cache[CACHE_CAP];
static size_t cache_size = 0;   /* next index to be computed */

/* Ensure that a(k) is computed for all k <= n */
static void ensure_cache(idx_t n)
{
    size_t idx = (size_t)n;
    if (idx >= CACHE_CAP) {
        abort(); /* n too large for the fixed cache */
    }

    /* Initialize base values once */
    if (cache_size == 0) {
        static const seq_t base[12] = {
            (seq_t)1, /* a(0) */
            (seq_t)0, /* a(1) */
            (seq_t)0, /* a(2) */
            (seq_t)0, /* a(3) */
            (seq_t)0, /* a(4) */
            (seq_t)1, /* a(5) */
            (seq_t)1, /* a(6) */
            (seq_t)1, /* a(7) */
            (seq_t)1, /* a(8) */
            (seq_t)1, /* a(9) */
            (seq_t)2, /* a(10) */
            (seq_t)3  /* a(11) */
        };
        for (size_t i = 0; i < 12; ++i) {
            cache[i] = base[i];
        }
        cache_size = 12; /* next index to fill */
    }

    /* Compute missing values up to idx */
    if (cache_size <= idx) {
        for (size_t i = cache_size; i <= idx; ++i) {
            seq_t sum = 0;
            for (int j = 5; j <= 11; ++j) {
                sum += cache[i - j];
            }
            cache[i] = sum;
        }
        cache_size = idx + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
