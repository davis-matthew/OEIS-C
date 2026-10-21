/*
 * A067115  Let N = 1357911131517192123252729... = concatenation of odd
 *          numbers. Then a(n) = sum of first n digits of N.
 * https://oeis.org/A067115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the sum of the first n digits of the concatenation * of
 *            all odd natural numbers: 1,3,5,7,9,11,13,15,…. The function *
 *            accumulates digit sums on demand, caching all intermediate
 *            values * so that subsequent or out‑of‑order calls are fast
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* cached values a(1)..a(cacheSize) */
    static seq_t *cache = NULL;
    static idx_t cacheSize = 0;   /* number of cached terms */
    static idx_t cacheCap  = 0;   /* allocated capacity */

    /* state of the ongoing digit stream */
    static uint64_t curOdd      = 1;   /* current odd number */
    static int     digitDigits[20];     /* digits of curOdd (little endian) */
    static int     digitCount   = 0;    /* number of digits in curOdd */
    static int     digitPos     = 0;    /* next digit to consume (from MSB) */
    static seq_t  cumSum        = 0;    /* sum of digits processed */
    static idx_t  totalDigits   = 0;    /* total digits processed */

    if (n == 0) return 0; /* undefined for n=0, return 0 as safe fallback */

    while (cacheSize < n) {
        /* (re)compute digits of curOdd when needed */
        if (digitPos == 0) {
            digitCount = 0;
            uint64_t tmp = curOdd;
            do {
                digitDigits[digitCount++] = tmp % 10;
                tmp /= 10;
            } while (tmp);
            digitPos = digitCount;           /* next digit will be most
                                                significant (last in array) */
        }

        /* consume next digit from most significant side */
        int digit = digitDigits[--digitPos];
        cumSum += (seq_t)digit;
        ++totalDigits;

        /* store the new a(totalDigits) into cache */
        if (cacheSize == cacheCap) {
            size_t newCap = cacheCap == 0 ? 64 : cacheCap * 2;
            seq_t *newCache = (seq_t *)malloc(newCap * sizeof(seq_t));
            if (newCache == NULL) exit(1);   /* allocation failed */
            if (cache != NULL) {
                for (idx_t i = 0; i < cacheSize; ++i)
                    newCache[i] = cache[i];
                free(cache);
            }
            cache = newCache;
            cacheCap = newCap;
        }
        cache[cacheSize++] = cumSum;

        /* move to next odd number once all digits of curOdd have been used */
        if (digitPos == 0)
            curOdd += 2;
    }
    return cache[n-1];
}
