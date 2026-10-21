/*
 * A063277  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          4 ).
 * https://oeis.org/A063277
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) – a(n-10) for n > 13, with a(2) = –1,
 *            a(3) = 0, a(4) = 0, a(5) = 0, a(6) = 1, a(7) = 1, a(8) = 2, a(9)
 *            = 0, a(10) = 3, a(11) = 1, a(12) = 4, a(13) = 1
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>    /* for size_t */

static seq_t cache[20001] = {0};   /* storage for 0 .. 20000 */
static idx_t max_precomputed = 13; /* highest index we have computed */
static int initialized = 0;        /* flag to set base values once */

static void init_cache(void)
{
    if (initialized) return;
    cache[2] = (seq_t)-1;
    cache[3] = (seq_t) 0;
    cache[4] = (seq_t) 0;
    cache[5] = (seq_t) 0;
    cache[6] = (seq_t) 1;
    cache[7] = (seq_t) 1;
    cache[8] = (seq_t) 2;
    cache[9] = (seq_t) 0;
    cache[10] = (seq_t) 3;
    cache[11] = (seq_t) 1;
    cache[12] = (seq_t) 4;
    cache[13] = (seq_t) 1;
    initialized = 1;
}

static void ensure_computed(idx_t n)
{
    if (n <= max_precomputed) return;
    if (n >= sizeof(cache)/sizeof(cache[0]))
        return; /* safety: limit our precomputation to the array size */

    for (idx_t i = max_precomputed + 1; i <= n; ++i) {
        if (i <= 13) {
            /* base values already set in init_cache */
            continue;
        }
        cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
    }
    max_precomputed = n;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0; /* sequence is defined for n >= 2 */
    init_cache();
    ensure_computed(n);
    return cache[n];
}
