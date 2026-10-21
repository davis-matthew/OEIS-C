/*
 * A235965  Number of (n+1)X(1+1) 0..3 arrays colored with the upper median
 *          value of each 2X2 subblock.
 * https://oeis.org/A235965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235965
 *
 *  a(1) = 256
 *  a(2) = 1964
 *  a(3) = 13194
 *  a(4) = 85476
 *  a(5) = 556486
 *  a(6) = 3614066
 *  a(7) = 23457361
 *  a(8) = 152354048
 *  a(9) = 990262160
 *  a(10) = 6430004688
 *  a(11) = 41756569624
 *  a(12) = 271256431352
 *  a(13) = 1762097787460
 *
 * For n > 13 the sequence satisfies the linear recurrence
 *
 *   a(n) = 96*a(n-3) + 752*a(n-4) + 1456*a(n-5) + 6180*a(n-6)
 *        + 6848*a(n-7) + 13712*a(n-8) + 9408*a(n-9) + 12288*a(n-10)
 *
 * The implementation memoises the values so that repeated calls
 * are O(1) after the first expansion.
 */

#include <stdlib.h>

static const seq_t init_vals[13] = {
    (seq_t)256,           /* a(1) */
    (seq_t)1964,          /* a(2) */
    (seq_t)13194,         /* a(3) */
    (seq_t)85476,         /* a(4) */
    (seq_t)556486,        /* a(5) */
    (seq_t)3614066,       /* a(6) */
    (seq_t)23457361,      /* a(7) */
    (seq_t)152354048,     /* a(8) */
    (seq_t)990262160,     /* a(9) */
    (seq_t)6430004688,    /* a(10) */
    (seq_t)41756569624,   /* a(11) */
    (seq_t)271256431352,  /* a(12) */
    (seq_t)1762097787460  /* a(13) */
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoised values, index 0 unused */
    static size_t cache_size = 0;    /* number of allocated elements (>=0) */
    static size_t computed_max = 0;  /* largest index that has a value */

    size_t idx = (size_t)n;          /* local copy for indexing */

    if (idx == 0) return (seq_t)0;   /* a(0) is undefined, return 0 */

    /* Grow the cache if necessary */
    if (cache_size < idx + 1) {
        size_t new_size = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise the first 13 terms if not already done */
    if (computed_max < 13) {
        size_t up = (computed_max < 13) ? 13 : computed_max;
        if (idx < 13) up = idx;
        for (size_t i = 1; i <= up; ++i) {
            cache[i] = init_vals[i - 1];
        }
        computed_max = up;
    }

    /* Compute new terms using the recurrence */
    if (idx > computed_max) {
        size_t start = (computed_max < 13) ? 14 : computed_max + 1;
        for (size_t i = start; i <= idx; ++i) {
            seq_t value = (cache[i - 3] * (seq_t)96) +
                          (cache[i - 4] * (seq_t)752) +
                          (cache[i - 5] * (seq_t)1456) +
                          (cache[i - 6] * (seq_t)6180) +
                          (cache[i - 7] * (seq_t)6848) +
                          (cache[i - 8] * (seq_t)13712) +
                          (cache[i - 9] * (seq_t)9408) +
                          (cache[i -10] * (seq_t)12288);
            cache[i] = value;
        }
        computed_max = idx;
    }

    return cache[idx];
}
