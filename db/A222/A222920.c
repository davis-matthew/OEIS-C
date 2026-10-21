/*
 * A222920  Number of nX2 0..4 arrays with no three equal values in a row
 *          horizontally or vertically, and new values 0..4 introduced in row
 *          major order.
 * https://oeis.org/A222920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A222920
 *  Recurrence (n > 11):
 *      a(n) = 28*a(n-1) -78*a(n-2) -789*a(n-3) +1250*a(n-4)
 *             +3796*a(n-5) -3288*a(n-6) -2208*a(n-7) +2432*a(n-8)
 *             -512*a(n-9)
 *  Initial values a(1)..a(11) are listed in the OEIS entry.
 *
 *  This implementation memoises the sequence up to the largest requested
 *  index, using a static array large enough for n up to 10000
 *  (the expected maximum).  All arithmetic is performed in seq_t,
 *  which is either an unsigned integer type of arbitrary size or an
 *  arbitrary–precision integer class – the code works for both.
 */

#include <stdint.h>   /* for uint8_t */

static const seq_t init_vals[11] = {
    (seq_t)2ULL,
    (seq_t)15ULL,
    (seq_t)174ULL,
    (seq_t)3183ULL,
    (seq_t)68076ULL,
    (seq_t)1543538ULL,
    (seq_t)35663142ULL,
    (seq_t)829049736ULL,
    (seq_t)19310392822ULL,
    (seq_t)450063924406ULL,
    (seq_t)10491666753024ULL
};

#define MAX_N 10000  /* supports n up to about 10000 */

static seq_t cache[MAX_N + 2];  /* 1‑based; cache[0] unused */
static idx_t cache_len = 0;     /* highest index that has been filled */

/*---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – behaviour for n < 1 irrelevant */

    /* Initialise the cache once, up to the 11th element */
    if (cache_len == 0) {
        for (idx_t i = 1; i <= 11; ++i) cache[i] = init_vals[i-1];
        cache_len = 11;
    }

    /* If the requested index is already computed, return it */
    if (n <= cache_len) return cache[n];

    /* Compute subsequent terms up to n */
    for (idx_t j = cache_len + 1; j <= n; ++j) {
        /* Compute the recurrence in two parts to avoid underflow on unsigned types */
        seq_t pos = 0;
        pos += (seq_t)28ULL  * cache[j-1];
        pos += (seq_t)1250ULL * cache[j-4];
        pos += (seq_t)3796ULL * cache[j-5];
        pos += (seq_t)2432ULL * cache[j-8];

        seq_t neg = 0;
        neg += (seq_t)78ULL  * cache[j-2];
        neg += (seq_t)789ULL  * cache[j-3];
        neg += (seq_t)3288ULL * cache[j-6];
        neg += (seq_t)2208ULL * cache[j-7];
        neg += (seq_t)512ULL  * cache[j-9];

        cache[j] = pos - neg;
    }
    cache_len = n;
    return cache[n];
}
