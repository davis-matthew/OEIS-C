/*
 * A221059  Equals one maps: number of n X 3 binary arrays indicating the
 *          locations of corresponding elements equal to exactly one of their
 *          horizontal and antidiagonal neighbors in a random 0..3 n X 3
 *          array.
 * https://oeis.org/A221059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 10*a(n-2) - 104*a(n-3) - 80*a(n-4) - 8*a(n-5)
 *            - 32*a(n-6), with initial terms a(1)=4, a(2)=37, a(3)=323,
 *            a(4)=2735, a(5)=22683, a(6)=185563
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0; /* number of allocated slots (index 0 unused) */

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n=0 */

    /* Ensure cache covers index n */
    if ((size_t)n >= cache_len) {
        size_t newlen = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failed */
        cache = tmp;

        /* initial known terms for indices 1..6 */
        static const seq_t init[7] = {
            0ULL,          /* index 0 unused */
            4ULL, 37ULL, 323ULL, 2735ULL, 22683ULL, 185563ULL
        };

        for (size_t i = cache_len; i < newlen; ++i) {
            if (i <= 6) {
                cache[i] = init[i];
            } else {
                cache[i] = (seq_t)11 * cache[i-1]
                           - (seq_t)10 * cache[i-2]
                           - (seq_t)104 * cache[i-3]
                           - (seq_t)80 * cache[i-4]
                           - (seq_t)8  * cache[i-5]
                           - (seq_t)32 * cache[i-6];
            }
        }
        cache_len = newlen;
    }
    return cache[(size_t)n];
}
