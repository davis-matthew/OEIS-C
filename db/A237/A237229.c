/*
 * A237229  Number of (n+1)X(3+1) 0..2 arrays with the maximum plus the
 *          minimum of every 2X2 subblock differing from its horizontal and
 *          vertical neighbors by exactly one.
 * https://oeis.org/A237229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 14, u64 n <= 34, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 5*a(n-2) + 42*a(n-4) - 21*a(n-6) - 207*a(n-8) for n > 13
 * Initial values: a(1)=928, a(2)=3284, a(3)=12014, a(4)=29456, a(5)=97862,
 *                 a(6)=269844, a(7)=926086, a(8)=2450048, a(9)=8256930,
 *                 a(10)=22301740, a(11)=75579798, a(12)=202641416, a(13)=684983946
 */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cached = NULL;        /* array of values, cached[0] unused */
static idx_t cached_len = 0;       /* number of stored elements (index+1) */

static const seq_t init_vals[14] = {
    0ull,                 /* a(0) unused */
    928ull, 3284ull, 12014ull, 29456ull, 97862ull,
    269844ull, 926086ull, 2450048ull, 8256930ull,
    22301740ull, 75579798ull, 202641416ull, 684983946ull
};

seq_t a(idx_t n)
{
    /* Return the pre‑computed initial values directly. */
    if (n <= 13)
        return init_vals[n];

    /* Initialise the cache on first use. */
    if (cached_len == 0) {
        cached_len = 14;  /* indices 0..13 */
        cached = (seq_t *)malloc(sizeof(seq_t) * cached_len);
        if (!cached)
            exit(1);
        for (idx_t i = 0; i < cached_len; ++i)
            cached[i] = init_vals[i];
    }

    /* Extend the cache if necessary. */
    if (cached_len <= n) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cached, sizeof(seq_t) * new_len);
        if (!tmp)
            exit(1);
        cached = tmp;
        for (idx_t k = cached_len; k <= n; ++k) {
            seq_t sum1 = (seq_t)5 * cached[k-2] + (seq_t)42 * cached[k-4];
            seq_t sum2 = (seq_t)21 * cached[k-6] + (seq_t)207 * cached[k-8];
            cached[k] = sum1 - sum2;          /* safe: sum1 >= sum2 for n > 13 */
        }
        cached_len = new_len;
    }

    return cached[n];
}
