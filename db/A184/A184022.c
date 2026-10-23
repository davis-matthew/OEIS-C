/*
 * A184022  1/8 the number of (n+1)X3 0..3 arrays with all 2X2 subblock sums
 *          the same.
 * https://oeis.org/A184022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence satisfies the 15‑step linear recurrence a(n) =
 *            8*a(n-1)+16*a(n-2)-268*a(n-3)+189*a(n-4)+3388*a(n-5)
 *            -5824*a(n-6)-20048*a(n-7)+49756*a(n-8)+52752*a(n-9)
 *            -196896*a(n-10)-24192*a(n-11)+369216*a(n-12)-131328*a(n-13)
 *            -262656*a(n-14)+165888*a(n-15) with initial values a(1)=88,
 *            a(2)=204, a(...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static const seq_t init[15] = {
    (seq_t)88,          /* a(1) */
    (seq_t)204,         /* a(2) */
    (seq_t)491,         /* a(3) */
    (seq_t)1347,        /* a(4) */
    (seq_t)3781,        /* a(5) */
    (seq_t)11541,       /* a(6) */
    (seq_t)35771,       /* a(7) */
    (seq_t)117321,      /* a(8) */
    (seq_t)389053,      /* a(9) */
    (seq_t)1343649,     /* a(10) */
    (seq_t)4678331,     /* a(11) */
    (seq_t)16789497,    /* a(12) */
    (seq_t)60610141,    /* a(13) */
    (seq_t)223849641,   /* a(14) */
    (seq_t)830159051    /* a(15) */
};

/* Cache large enough for n up to 10000 (the hard upper bound for timing). */
#define MAX_N 10001
static seq_t cache[MAX_N];
static idx_t cache_len = 0;   /* number of computed entries, initially 0 */

/* Ensure that cache contains a(1)..a(n) for n>=1. */
static void ensure_cache(idx_t n)
{
    if (n == 0 || n <= cache_len) return;

    /* First fill initial values if not already present. */
    if (cache_len == 0) {
        for (idx_t i = 0; i < 15; ++i) {
            cache[i] = init[i];
        }
        cache_len = 15;
    }

    /* Compute terms up to n using the recurrence. */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        /* k is 1‑based.  cache indices are 0‑based. */
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;

        pos += (seq_t)8      * cache[k - 2];
        pos += (seq_t)16     * cache[k - 3];
        pos += (seq_t)189    * cache[k - 5];
        pos += (seq_t)3388   * cache[k - 6];
        pos += (seq_t)49756  * cache[k - 9];
        pos += (seq_t)52752  * cache[k -10];
        pos += (seq_t)369216 * cache[k -13];
        pos += (seq_t)165888 * cache[k -16];

        neg += (seq_t)268    * cache[k - 4];
        neg += (seq_t)5824   * cache[k - 7];
        neg += (seq_t)20048  * cache[k - 8];
        neg += (seq_t)196896 * cache[k -11];
        neg += (seq_t)24192  * cache[k -12];
        neg += (seq_t)131328 * cache[k -14];
        neg += (seq_t)262656 * cache[k -15];

        cache[k - 1] = pos - neg;
    }

    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n <= 15) return init[n - 1];
    ensure_cache(n);
    return cache[n - 1];
}
