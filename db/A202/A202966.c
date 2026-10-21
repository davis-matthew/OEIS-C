/*
 * A202966  Number of arrays of 6 integers in -n..n with sum zero and adjacent
 *          elements differing in absolute value.
 * https://oeis.org/A202966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) -3*a(n-4) +a(n-5) +a(n-6) +a(n-9) +a(n-10) -
 *            3*a(n-11) +2*a(n-14) -a(n-15) (initial values) a(1)=0, a(2)=200,
 *            a(3)=1936, a(4)=9884, a(5)=33736, a(6)=90604, a(7)=207184,
 *            a(8)=421620, a(9)=785504, a(10)=1366876, a(11)=2251016,
 *            a(12)=3543320, a(13)=5371856, a(14)=7888384, a(15)=11270788
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 48, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t last_computed = 0;
    static bool initialized = false;

    if (n == 0)
        return (seq_t)0;   /* undefined for n=0 */

    /* initialise cache with first 15 terms plus a(0)=0 */
    if (!initialized) {
        cache_cap = 16;   /* indices 0 .. 15 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;
        /* a(0) is unused but zeroed for safety */
        cache[0] = (seq_t)0;
        cache[1]  = (seq_t)0;
        cache[2]  = (seq_t)200;
        cache[3]  = (seq_t)1936;
        cache[4]  = (seq_t)9884;
        cache[5]  = (seq_t)33736;
        cache[6]  = (seq_t)90604;
        cache[7]  = (seq_t)207184;
        cache[8]  = (seq_t)421620;
        cache[9]  = (seq_t)785504;
        cache[10] = (seq_t)1366876;
        cache[11] = (seq_t)2251016;
        cache[12] = (seq_t)3543320;
        cache[13] = (seq_t)5371856;
        cache[14] = (seq_t)7888384;
        cache[15] = (seq_t)11270788;
        last_computed = 15;
        initialized = true;
    }

    /* grow cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;
        /* initialise new slots to 0 (defensive) */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    if (n <= last_computed)
        return cache[n];

    /* compute terms from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t pos = cache[i-1] * ((seq_t)2);
        pos += cache[i-5];
        pos += cache[i-6];
        pos += cache[i-9];
        pos += cache[i-10];
        pos += cache[i-14] * ((seq_t)2);

        seq_t neg = cache[i-4] * ((seq_t)3);
        neg += cache[i-11] * ((seq_t)3);
        neg += cache[i-15];

        cache[i] = pos - neg;
    }

    last_computed = n;
    return cache[n];
}
