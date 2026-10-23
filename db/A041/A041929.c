/*
 * A041929  Denominators of continued fraction convergents to sqrt(487).
 * https://oeis.org/A041929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(487).
   Recurrence:
        a(0) = 1
        a(n) = a_n * a(n‑1) + a(n‑2)   for n ≥ 1
   where a_n are the periodic partial quotients of sqrt(487)
   (period length 28: a_1 = 14, a_2 = 15, …).  The sequence is memoised
   so calls in any order run in O(n) time. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* memoised terms */
static size_t cache_size = 0;        /* number of cached terms */

static uint64_t period[28];          /* a_1 … a_28 */
static bool period_ready = false;    /* once‑only initialization */
static const uint64_t S = 487ULL;
static const uint64_t A0 = 22ULL;    /* floor(sqrt(487)) */

static void compute_period(void)
{
    if (period_ready) return;
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = A0;
    for (int i = 0; i < 28; ++i) {
        m = d * a - m;
        d = (S - m * m) / d;
        a = (A0 + m) / d;
        period[i] = a;           /* a_{i+1} */
    }
    period_ready = true;
}

/* Ensure cache contains terms 0 … n inclusive. */
static void extend_cache(idx_t n)
{
    size_t want = (size_t)n + 1;
    if (want <= cache_size) return;          /* already computed */

    /* Allocate or grow the cache. */
    cache = (seq_t*)realloc(cache, want * sizeof(seq_t));
    if (!cache) return;                      /* allocation failure ignored */

    /* Generate the missing terms. */
    for (size_t i = cache_size; i < want; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }
        compute_period();                    /* ensure period[] is ready */

        uint64_t ai = period[(i - 1) % 28];   /* partial quotient a_i */
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = (i >= 2) ? cache[i - 2] : (seq_t)0;
        cache[i] = (seq_t)ai * prev1 + prev2;
    }
    cache_size = want;
}

seq_t a(idx_t n)
{
    extend_cache(n);
    return cache[(size_t)n];
}
