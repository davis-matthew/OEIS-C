/*
 * A119837  a(n) = (2n)!/n! - (2n)!/(n-1)!.
 * https://oeis.org/A119837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (2n)!/n! - (2n)!/(n-1)! = -(n-1) * (2n)!/n!
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for computed values, grows as needed */
    static seq_t *cache = NULL;      /* cache[0 .. curr_n] */
    static idx_t  curr_n = 0;        /* highest n computed */
    static seq_t  R = 1;             /* R = (2*curr_n)! / curr_n!  */
    /* initialize cache on first call */
    if (cache == NULL) {
        /* allocate space for n+1 entries (n could be 0) */
        size_t sz = (size_t)(n + 1);
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (cache == NULL) {
            /* In case of allocation failure, terminate the program */
            abort();
        }
        /* base case: a(0) = 1, R is 1 */
        cache[0] = 1;
        curr_n = 0;
        R = 1;
    }
    /* If requested n is already cached, return it immediately */
    if (n <= curr_n) {
        return cache[n];
    }
    /* Ensure the cache can hold up to n */
    if (cache && (size_t)(n + 1) > (size_t)(curr_n + 1)) {
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (tmp == NULL) {
            abort();
        }
        cache = tmp;
    }
    /* Compute values incrementally from curr_n+1 up to n */
    for (idx_t k = curr_n + 1; k <= n; ++k) {
        /* R_{k} = R_{k-1} * (2k-1)*(2k)/k  (exact division) */
        seq_t num = (seq_t)(2 * k - 1) * (seq_t)(2 * k);
        R = (R * num) / (seq_t)k;
        /* a(k) = -(k-1) * R */
        cache[k] = -( (seq_t)(k - 1) ) * R;
    }
    curr_n = n;
    return cache[n];
}
