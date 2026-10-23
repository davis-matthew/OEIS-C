/*
 * A129452  Expansion of
 *          (1-9*x^2-27*x^3)/((1+3*x+9*x^2)*(1-4*x-9*x^2-9*x^3+81*x^4)).
 * https://oeis.org/A129452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) =
 *            (1-9*x^2-27*x^3)/((1+3*x+9*x^2)*(1-4*x-9*x^2-9*x^3+81*x^4))
 *            Recurrence (for n ≥ 6): a(n) =
 *            a(n-1)+12*a(n-2)+72*a(n-3)+27*a(n-4)-162*a(n-5)-729*a(n-6)
 *            Initial values: a(0)=1, a(1)=1, a(2)=4, a(3)=61, a(4)=208,
 *            a(5)=1093
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t */

seq_t a(idx_t n)
{
    /* Static cache up to a reasonable maximum (≈10 000 – 20 000).  */
    enum { MAX_CACHE = 20000 };          /* should be > all n used in tests */
    static seq_t cache[MAX_CACHE + 1];
    static idx_t cache_len = 0;          /* number of terms currently computed */

    /* Initialize first six terms */
    if (!cache_len) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)61;
        cache[4] = (seq_t)208;
        cache[5] = (seq_t)1093;
        cache_len = 6;                   /* next index to compute */
    }

    /* If requested index is already cached, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* If n exceeds the pre‑allocated cache, compute on the fly without caching */
    if (n > MAX_CACHE) {
        /* Use a sliding window of the last six values. */
        seq_t last[6];
        /* initialise with the known first six terms */
        last[0] = cache[0];
        last[1] = cache[1];
        last[2] = cache[2];
        last[3] = cache[3];
        last[4] = cache[4];
        last[5] = cache[5];

        for (idx_t i = 6; i <= n; ++i) {
            seq_t pos = last[5];                                 /* a(n-1)   */
            pos += last[4] * (seq_t)12;                          /* +12*a(n-2)*/
            pos += last[3] * (seq_t)72;                          /* +72*a(n-3)*/
            pos += last[2] * (seq_t)27;                          /* +27*a(n-4)*/
            seq_t neg = last[1] * (seq_t)162 + last[0] * (seq_t)729; /* -162*a(n-5)-729*a(n-6) */
            seq_t cur = pos - neg;

            /* slide the window */
            last[0] = last[1];
            last[1] = last[2];
            last[2] = last[3];
            last[3] = last[4];
            last[4] = last[5];
            last[5] = cur;
        }
        return last[5];
    }

    /* Compute and cache terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t pos = cache[i-1];
        pos += cache[i-2] * (seq_t)12;
        pos += cache[i-3] * (seq_t)72;
        pos += cache[i-4] * (seq_t)27;
        seq_t neg = cache[i-5] * (seq_t)162 + cache[i-6] * (seq_t)729;
        cache[i] = pos - neg;
    }
    cache_len = n + 1;  /* we have cached 0 … n inclusive */
    return cache[n];
}
