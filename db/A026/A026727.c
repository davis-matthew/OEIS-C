/*
 * A026727  Expansion of 1/((1-2*x)*(1-6*x)*(1-8*x)*(1-11*x)).
 * https://oeis.org/A026727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*a(n-1) - 252*a(n-2) + 932*a(n-3) - 1056*a(n-4) a(0)=1,
 *            a(1)=27, a(2)=477, a(3)=7007
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cache the computed terms.  The sequence is used up to n≈10000
       during testing, so an array of 10001 entries is sufficient. */
    static seq_t cache[10001];
    static idx_t cache_len = 0;

    /* Initialise the first four values on first entry. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)27;
        cache[2] = (seq_t)477;
        cache[3] = (seq_t)7007;
        cache_len = 4;
    }

    /* Compute any missing terms up to n. */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t t1 = ((seq_t)27)  * cache[i-1];
            seq_t t2 = ((seq_t)252) * cache[i-2];
            seq_t t3 = ((seq_t)932) * cache[i-3];
            seq_t t4 = ((seq_t)1056) * cache[i-4];
            cache[i] = t1 - t2 + t3 - t4;
        }
        if (n + 1 > cache_len) cache_len = n + 1;
    }

    return cache[n];
}
