/*
 * A042221  Denominators of continued fraction convergents to sqrt(636).
 * https://oeis.org/A042221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7011902*a(n-12) - a(n-24) for n>23
   Initial values for n=0..23 are:
   1,4,5,9,32,105,1292,3981,13235,17216,30451,139020,
   6981451,28064824,35046275,63111099,224379572,736249815,
   9059377352,27914381871,92802522965,120716904836,
   213519427801,974794616040
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  len   = 0;

    static const seq_t init[24] = {
        1ULL,  4ULL,  5ULL,  9ULL, 32ULL, 105ULL,
        1292ULL, 3981ULL, 13235ULL, 17216ULL,
        30451ULL, 139020ULL, 6981451ULL, 28064824ULL,
        35046275ULL, 63111099ULL, 224379572ULL, 736249815ULL,
        9059377352ULL, 27914381871ULL, 92802522965ULL,
        120716904836ULL, 213519427801ULL, 974794616040ULL
    };

    /* Ensure cache can hold up to index n */
    if (n >= cap) {
        size_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* Load the first 24 terms if not already loaded */
    if (len < 24) {
        idx_t i;
        for (i = len; i < 24 && i <= n; ++i) {
            cache[i] = init[i];
        }
        if (i > len) len = i;
    }

    if (n < len) return cache[n];

    /* Compute remaining terms using the recurrence */
    for (idx_t i = len; i <= n; ++i) {
        /* a(i) = 7011902*a(i-12) - a(i-24) */
        cache[i] = (seq_t)7011902 * cache[i - 12] - cache[i - 24];
    }
    len = n + 1;
    return cache[n];
}
