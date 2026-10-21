/*
 * A203918  Number of (n+2)X5 0..1 arrays with every 3X3 subblock having equal
 *          diagonal elements or equal antidiagonal elements, and new values
 *          0..1 introduced in row major order.
 * https://oeis.org/A203918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies * a(n) = 3*a(n-1) + 7*a(n-3) - 11*a(n-4) +
 *            34*a(n-5) + 46*a(n-6) * - 74*a(n-7) - 78*a(n-8) - 92*a(n-9) +
 *            92*a(n-10) + 92*a(n-11) * - 44*a(n-12) + 40*a(n-13) + 32*a(n-15)
 *            - 16*a(n-16) (n>17) * with initial values a(1)=1400, a(2)=6050,
 *            …, a(17)=1085227801658. * The implementation mem...
 * Range:     u8 none, u16 n <= 3, u32 n <= 12, u64 n <= 30, u128 n <= 65,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                  /* for size_t */
#include "../utils/grow_seq.h"       /* for grow_seq(seq_t **, size_t *, size_t) */

/* External type definitions are supplied by the build system. */
extern seq_t a(idx_t n) {
    /* Cache keeps a(i) in cache[i] for i ≥ 1; cache[0] unused. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;    /* capacity of the array (number of slots) */
    static size_t cache_hi = 1;     /* first index that is NOT yet known */

    if (n < 1) return 0;           /* undefined for n < 1, follow OEIS convention */

    /* Ensure the array is large enough for index n. */
    size_t need = (size_t)n + 1;   /* we want slots 0..n inclusive */
    if (cache_cap < need) {
        grow_seq(&cache, &cache_cap, need);
    }

    /* initialise the first 17 terms once */
    if (cache_hi == 1) {
        /* guarantee at least 18 slots (0..17). */
        if (cache_cap < 18) grow_seq(&cache, &cache_cap, 18);

        cache[0] = 0;                       /* unused */
        cache[1] = (seq_t)1400;
        cache[2] = (seq_t)6050;
        cache[3] = (seq_t)21470;
        cache[4] = (seq_t)75272;
        cache[5] = (seq_t)267066;
        cache[6] = (seq_t)949442;
        cache[7] = (seq_t)3376768;
        cache[8] = (seq_t)12014802;
        cache[9] = (seq_t)42694542;
        cache[10] = (seq_t)151658528;
        cache[11] = (seq_t)538889650;
        cache[12] = (seq_t)1914938498;
        cache[13] = (seq_t)6805082304;
        cache[14] = (seq_t)24183722738;
        cache[15] = (seq_t)85939263318;
        cache[16] = (seq_t)305389881288;
        cache[17] = (seq_t)1085227801658;
        cache_hi = 18;                      /* computed up to index 17 */
    }

    /* compute any missing terms up to n */
    for (size_t i = cache_hi; i <= (size_t)n; ++i) {
        if (i <= 17) continue;              /* already known */

        seq_t sum = 0;
        sum += 3 * cache[i-1];
        sum += 7 * cache[i-3];
        sum -= 11 * cache[i-4];
        sum += 34 * cache[i-5];
        sum += 46 * cache[i-6];
        sum -= 74 * cache[i-7];
        sum -= 78 * cache[i-8];
        sum -= 92 * cache[i-9];
        sum += 92 * cache[i-10];
        sum += 92 * cache[i-11];
        sum -= 44 * cache[i-12];
        sum += 40 * cache[i-13];
        if (i >= 15) sum += 32 * cache[i-15];
        sum -= 16 * cache[i-16];
        cache[i] = sum;
    }

    cache_hi = (size_t)n + 1;              /* all up to n are now known */
    return cache[(size_t)n];
}
