/*
 * A237318  Number of (n+1)X(2+1) 0..2 arrays with the maximum plus the upper
 *          median plus the minimum minus the lower median of every 2X2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1400*a(n-4) - 6519*a(n-8) + 126*a(n-12) (n>14) * Initial
 *            values: a(1)=348, a(2)=2108, a(3)=12756, a(4)=79360, *
 *            a(5)=485484, a(6)=2938226, a(7)=17798196, a(8)=110739192, *
 *            a(9)=677413956, a(10)=4099776412, a(11)=24834317148, *
 *            a(12)=154517525352, a(13)=945214707012, a(14)=5720532944090
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* 0‑based cache of a(1)…; cache[i] holds a(i+1) */
static seq_t *cache    = NULL;
static size_t cache_sz = 0;  /* number of initialized entries */

/* first 14 values (index 0 unused) */
static const seq_t init_vals[15] = {
    (seq_t)0,               /* dummy for index 0 */
    (seq_t)348,             /* a(1)  */
    (seq_t)2108,            /* a(2)  */
    (seq_t)12756,           /* a(3)  */
    (seq_t)79360,           /* a(4)  */
    (seq_t)485484,          /* a(5)  */
    (seq_t)2938226,         /* a(6)  */
    (seq_t)17798196,        /* a(7)  */
    (seq_t)110739192,       /* a(8)  */
    (seq_t)677413956,       /* a(9)  */
    (seq_t)4099776412,      /* a(10) */
    (seq_t)24834317148,     /* a(11) */
    (seq_t)154517525352,    /* a(12) */
    (seq_t)945214707012,    /* a(13) */
    (seq_t)5720532944090    /* a(14) */
};

/* ensure the cache can hold at least n entries (n>=1) */
static void ensure_cache(idx_t n)
{
    if (n <= (idx_t)cache_sz)
        return;
    size_t new_sz = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);   /* allocation failure */
    cache = tmp;
    /* cache_sz is adjusted later after filling missing entries */
}

/* return a(n) for n >= 1 (undefined behaviour for n==0) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* graceful for indices < 1 */

    size_t needed = (size_t)n;               /* number of terms to store */
    ensure_cache(needed);

    /* previous number of initialized entries */
    size_t prev_sz = cache_sz;

    /* fill missing entries up to n */
    for (size_t i = prev_sz; i < needed; ++i) {
        if (i < 14) {
            /* i is 0-based, a(i+1) = init_vals[i+1] */
            cache[i] = init_vals[i + 1];
        } else {
            /* recurrence: a(i+1) = 1400*a(i-3) - 6519*a(i-7) + 126*a(i-11) */
            seq_t term1 = 1400 * cache[i - 4];   /* a(i-3)  */
            seq_t term2 = -6519 * cache[i - 8];  /* a(i-7)  */
            seq_t term3 = 126 * cache[i - 12];   /* a(i-11) */
            cache[i] = term1 + term2 + term3;
        }
    }

    cache_sz = needed;               /* update number of initialized entries */
    return cache[n - 1];              /* a(1) is at cache[0] */
}
