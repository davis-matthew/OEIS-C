/*
 * A235272  Number of (n+1) X (2+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 5
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 22, u64 n <= 46, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A235272 sequence:  Number of (n+1) X 3 arrays with the stated
   diagonal‑difference property.  The sequence satisfies the
   linear recurrence (offset 1):

   a(n) = a(n-1) +15*a(n-2) –15*a(n-3) –80*a(n-4) +80*a(n-5)
          +180*a(n-6) –180*a(n-7) –144*a(n-8) +144*a(n-9)
   for n ≥ 10, with initial values

   a(1)=100, a(2)=208, a(3)=380, a(4)=844, a(5)=1660,
   a(6)=3844, a(7)=8012, a(8)=19060, a(9)=41500.
*/

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised terms, indexed by n (0 unused) */
static size_t cache_len = 0;     /* maximum index currently computed, 0 if none */

/*  Initial values (index 1..9) */
static const seq_t init[9] = {
    100,
    208,
    380,
    844,
    1660,
    3844,
    8012,
    19060,
    41500
};

seq_t a(idx_t n)
{
    /* handle the trivial case n < 1 – undefined, return 0 */
    if (n < 1) return 0;

    /* if cached, return immediately */
    if (n <= cache_len) return cache[n];

    /* ensure enough storage */
    size_t needed = (size_t)n + 1;
    if (cache == NULL) {
        cache = (seq_t *)calloc(needed, sizeof(seq_t));
    } else if (needed > cache_len + 1) {
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    }

    /* fill missing initial terms */
    size_t i;
    for (i = cache_len + 1; i <= (size_t)9 && i <= n; ++i) {
        cache[i] = init[i - 1];
    }

    /* compute terms from cache_len+1 up to n */
    for (; i <= n; ++i) {
        /* i is at least 10 here */
        seq_t pos = cache[i - 1];                          /* a(i-1) */
        pos += 15 * cache[i - 2];                          /* +15*a(i-2) */
        pos += 80 * cache[i - 5];                          /* +80*a(i-5) */
        pos += 180 * cache[i - 6];                         /* +180*a(i-6) */
        pos += 144 * cache[i - 9];                         /* +144*a(i-9) */

        seq_t neg = 15 * cache[i - 3];                     /* -15*a(i-3) */
        neg += 80 * cache[i - 4];                          /* -80*a(i-4) */
        neg += 180 * cache[i - 7];                         /* -180*a(i-7) */
        neg += 144 * cache[i - 8];                         /* -144*a(i-8) */

        cache[i] = pos - neg;                              /* combine */
    }

    cache_len = (size_t)n;
    return cache[n];
}
