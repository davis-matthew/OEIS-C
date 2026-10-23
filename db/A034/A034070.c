/*
 * A034070  Decimal part of a(n)^(1/5) starts with n so that a(n)<a(n+1).
 * https://oeis.org/A034070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest integer a > a(n-1) whose decimal part of
 *            a^(1/5) begins with the decimal digits of n. (Sequence A034070,
 *            offset 0.)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 9, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"

/* Compute floor of the fifth root of x
   (maximum r such that r^5 <= x). */
static idx_t fifth_root_floor(seq_t x)
{
    idx_t low = 0, high = 1;
    /* Find an upper bound for the root */
    while (pow_seq((seq_t)high, 5) <= x) {
        high <<= 1;
    }
    /* Binary search */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (pow_seq((seq_t)mid, 5) <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Return the number of decimal digits of n (n >= 0). */
static int digit_count_idx(idx_t n)
{
    if (n == 0) return 1;
    int d = 0;
    while (n) {
        n /= 10;
        ++d;
    }
    return d;
}

/* Find the next value a(n) given the previous value a(n‑1) and the index n. */
static seq_t find_next_candidate(seq_t prev_a, idx_t n)
{
    int d = digit_count_idx(n);
    seq_t pow10k   = pow_seq((seq_t)10, d);   /* 10^d      */
    seq_t pow10_5k = pow_seq(pow10k, 5);      /* 10^(5d)   */

    seq_t start = prev_a + (seq_t)1;          /* candidate start */

    idx_t b = fifth_root_floor(prev_a);       /* starting integer part of the 5th root */

    while (1) {
        /* Compute integer bases for the interval */
        seq_t base1 = ((seq_t)b) * pow10k + (seq_t)n;         /* m*10^d + n   */
        seq_t base2 = base1 + (seq_t)1;                       /* m*10^d + n+1 */

        seq_t num1 = pow_seq(base1, 5);  /* (m*10^d + n)^5          */
        seq_t num2 = pow_seq(base2, 5);  /* (m*10^d + n + 1)^5      */

        seq_t L = (num1 + pow10_5k - (seq_t)1) / pow10_5k;     /* ceil(low^5)  */
        seq_t U = (num2 - (seq_t)1) / pow10_5k;                /* floor(high^5) */

        seq_t cand = (start > L) ? start : L;
        if (cand <= U)
            return cand;

        ++b;   /* next block of values */
    }
}

/* Cache for memoised terms of the sequence. */
static seq_t *cache    = NULL;
static size_t cache_len = 0;   /* actual length */
static size_t cache_cap = 0;   /* allocated capacity */

/* Main sequence routine. */
seq_t a(idx_t n)
{
    if (n < cache_len)
        return cache[n];

    /* Increase capacity if necessary. */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms in order. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t value;
        if (i == 0) {
            value = (seq_t)33;   /* a(0) = 33 */
        } else {
            value = find_next_candidate(cache[i - 1], i);
        }
        cache[i] = value;
        if ((size_t)i + 1 > cache_len)
            cache_len = (size_t)i + 1;
    }

    return cache[n];
}
