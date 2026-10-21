/*
 * A210079  Number of (n+1) X 2 0..3 arrays containing all values 0..3 with
 *          every 2 X 2 subblock having one, three or four distinct values,
 *          and new values 0..3 introduced in row major order.
 * https://oeis.org/A210079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*a(n-1) - 71*a(n-2) + 67*a(n-3) - 8*a(n-4) - 4*a(n-5)
 *            Initial values: a(1)=1, a(2)=51, a(3)=762, a(4)=9405,
 *            a(5)=109194, a(6)=1239185 Recurrence valid for n ≥ 7. The
 *            function is memoised and works for any order of calls. Unsigned
 *            arithmetic is safe because the leading term dominates all...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static const seq_t base_vals[6] = {
    (seq_t)1,
    (seq_t)51,
    (seq_t)762,
    (seq_t)9405,
    (seq_t)109194,
    (seq_t)1239185
};

static seq_t *cache = NULL;      /* cached values, index 1..n inclusive */
static size_t cache_size = 0;     /* allocated array size */
static idx_t cached = 0;         /* highest index computed */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */

    if (n <= 6) return base_vals[n - 1];

    /* Allocate or enlarge cache if necessary */
    size_t needed = (size_t)n + 1;        /* we use index n */
    if (needed > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) {                       /* allocation failed */
            return (seq_t)0;
        }
        cache = tmp;
        cache_size = needed;
    }

    /* Initialise base values once */
    if (cached < 6) {
        for (idx_t i = 1; i <= 6; ++i) cache[i] = base_vals[i - 1];
        cached = 6;
    }

    /* Compute further values using the recurrence */
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t t1 = mul_small(cache[i - 1], 17);
        seq_t t2 = mul_small(cache[i - 2], 71);
        seq_t t3 = mul_small(cache[i - 3], 67);
        seq_t t4 = mul_small(cache[i - 4], 8);
        seq_t t5 = mul_small(cache[i - 5], 4);

        seq_t res = t1;
        res -= t2;
        res += t3;
        res -= t4;
        res -= t5;
        cache[i] = res;
    }
    cached = n;

    return cache[n];
}
