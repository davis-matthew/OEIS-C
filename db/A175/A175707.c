/*
 * A175707  Number of ways to put n copies of 1,2,3,4 into sets.
 * https://oeis.org/A175707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   linear recurrence of order 18 a(n) = 7*a(n-1) -17*a(n-2)
 *            +8*a(n-3) +36*a(n-4) -60*a(n-5) +4*a(n-6) +56*a(n-7) -22*a(n-8)
 *            -22*a(n-9) -22*a(n-10) +56*a(n-11) +4*a(n-12) -60*a(n-13)
 *            +36*a(n-14) +8*a(n-15) -17*a(n-16) +7*a(n-17) -a(n-18) Initial
 *            values: a(0)=1, a(1)=15, a(2)=139, a(3)=862, a(4)=4079,...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 23, u64 n <= 202, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialisation: first 18 terms */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0]  = 1;
        cache[1]  = 15;
        cache[2]  = 139;
        cache[3]  = 862;
        cache[4]  = 4079;
        cache[5]  = 15791;
        cache[6]  = 52450;
        cache[7]  = 154279;
        cache[8]  = 411180;
        cache[9]  = 1009741;
        cache[10] = 2314278;
        cache[11] = 5000125;
        cache[12] = 10264997;
        cache[13] = 20152950;
        cache[14] = 38037517;
        cache[15] = 69323949;
        cache[16] = 122448455;
        cache[17] = 210271756;
        cache_len = 18;
    }

    /* return already known value */
    if ((size_t)n < cache_len)
        return cache[n];

    /* compute values up to n */
    while (cache_len <= (size_t)n) {
        /* grow cache if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        size_t i = cache_len;   /* index of the new term */

        /* compute positive and negative contributions separately
           to avoid underflow when using unsigned seq_t */
        seq_t pos = 0, neg = 0;
        pos += 7 * cache[i-1];
        pos += 8 * cache[i-3];
        pos += 36 * cache[i-4];
        pos += 4 * cache[i-6];
        pos += 56 * cache[i-7];
        pos += 56 * cache[i-11];
        pos += 4 * cache[i-12];
        pos += 36 * cache[i-14];
        pos += 8 * cache[i-15];
        pos += 7 * cache[i-17];

        neg += 17 * cache[i-2];
        neg += 60 * cache[i-5];
        neg += 22 * cache[i-8];
        neg += 22 * cache[i-9];
        neg += 22 * cache[i-10];
        neg += 60 * cache[i-13];
        neg += 17 * cache[i-16];
        neg += cache[i-18];   /* coefficient -1 */

        cache[i] = pos - neg;
        cache_len++;
    }

    return cache[n];
}
