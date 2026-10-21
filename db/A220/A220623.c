/*
 * A220623  Number of ways to reciprocally link elements of an 4Xn array
 *          either to themselves or to exactly one horizontal, diagonal and
 *          antidiagonal neighbor.
 * https://oeis.org/A220623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1)+98*a(n-2)-452*a(n-3)-102*a(n-4)+876*a(n-5) *
 *            -144*a(n-6)-268*a(n-7)+22*a(n-8)-8*a(n-9)+15*a(n-10) * with seed
 *            values * a(1)=1, a(2)=71, a(3)=765, a(4)=17084, a(5)=282777, *
 *            a(6)=5280827, a(7)=93905509, a(8)=1703300073, a(9)=30648000256,
 *            * a(10)=553256525421
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_from_str.h" /* provides seq_from_str() */

seq_t a(idx_t n)
{
    /* 0 is undefined; simply return 0 for n < 1 */
    if (n < 1) return (seq_t)0;

    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 0;

    /* First call – allocate and seed */
    if (!cache) {
        cache_cap = 11;                      /* indices 0 … 10 (0 unused) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* OOM – graceful fail */

        cache[0] = (seq_t)0;
        cache[1] = seq_from_str("1");
        cache[2] = seq_from_str("71");
        cache[3] = seq_from_str("765");
        cache[4] = seq_from_str("17084");
        cache[5] = seq_from_str("282777");
        cache[6] = seq_from_str("5280827");
        cache[7] = seq_from_str("93905509");
        cache[8] = seq_from_str("1703300073");
        cache[9] = seq_from_str("30648000256");
        cache[10]= seq_from_str("553256525421");
        max_computed = 10;
    }

    /* Grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* OOM */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If the value is already known, return it */
    if (n <= max_computed) return cache[n];

    /* Compute values from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = (seq_t)0;
        val += (seq_t)14 * cache[i-1];
        val += (seq_t)98 * cache[i-2];
        val -= (seq_t)452 * cache[i-3];
        val -= (seq_t)102 * cache[i-4];
        val += (seq_t)876 * cache[i-5];
        val -= (seq_t)144 * cache[i-6];
        val -= (seq_t)268 * cache[i-7];
        val += (seq_t)22  * cache[i-8];
        val -= (seq_t)8   * cache[i-9];
        val += (seq_t)15  * cache[i-10];
        cache[i] = val;
    }

    max_computed = n;
    return cache[n];
}
