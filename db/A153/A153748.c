/*
 * A153748  Numbers k such that there are 10 digits in k^2 and for each factor
 *          f of 10 (1, 2, 5) the sum of digit groupings of size f is a
 *          square.
 * https://oeis.org/A153748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 26, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: Numbers k such that k^2 has exactly 10 decimal digits and, for each
 * factor f of 10 (1, 2, 5), the sum of the digit groupings of size f of k^2
 * is a perfect square.
 * This implementation precomputes all such k in the range [ceil(sqrt(10^9)),
 * floor(sqrt(10^10-1))] and returns the nth element (n≥1).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"        /* floor_sqrt(seq_t) */

static seq_t *cache = NULL;          /* storage for found k's   */
static size_t cache_len = 0;         /* number of terms stored  */
static bool cache_initialized = false;

/* check whether the 10‑digit square satisfies the digit‑group sum condition */
static bool valid_square(seq_t sq)
{
    /* a. digit sum (f = 1) */
    seq_t t = sq;
    seq_t sum1 = 0;
    int dcnt = 0;
    while (t > 0 && dcnt < 10) {
        sum1 += t % 10;
        t /= 10;
        ++dcnt;
    }
    seq_t r = floor_sqrt(sum1);
    if (r * r != sum1)
        return false;

    /* b. pair sums (f = 2) */
    t = sq;
    seq_t sum2 = 0;
    int paircnt = 0;
    while (t > 0 && paircnt < 5) {
        sum2 += t % 100;
        t /= 100;
        ++paircnt;
    }
    r = floor_sqrt(sum2);
    if (r * r != sum2)
        return false;

    /* c. 5‑digit block sums (f = 5) */
    seq_t block1 = sq % 100000;
    seq_t block2 = sq / 100000;
    seq_t sum5  = block1 + block2;
    r = floor_sqrt(sum5);
    if (r * r != sum5)
        return false;

    return true;
}

/* generate all k in [ceil(sqrt(10^9)), floor(sqrt(10^10-1))] satisfying the condition */
static void initialise_cache(void)
{
    if (cache_initialized)
        return;

    const uint64_t mink = 31623; /* ceil(sqrt(1e9))   */
    const uint64_t maxk = 99999; /* floor(sqrt(1e10-1)) */

    const size_t max_possible = (size_t)(maxk - mink + 1);
    cache = (seq_t *)malloc(max_possible * sizeof(seq_t));
    if (!cache) {                /* allocation failed, leave empty cache */
        cache_len = 0;
        cache_initialized = true;
        return;
    }

    size_t cnt = 0;
    for (uint64_t k = mink; k <= maxk; ++k) {
        uint64_t sq64 = k * k;          /* 10‑digit square fits in 64 bits */
        seq_t sq = (seq_t)sq64;
        if (valid_square(sq))
            cache[cnt++] = (seq_t)k;
    }
    cache_len = cnt;
    cache_initialized = true;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n = 0, return 0 */

    initialise_cache();

    size_t idx = (size_t)(n - 1);
    if (idx >= cache_len)
        return (seq_t)0;                 /* beyond known terms */

    return cache[idx];
}
