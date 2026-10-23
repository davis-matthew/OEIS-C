/*
 * A184778  Numbers k such that 2k + floor(k*sqrt(2)) is prime.
 * https://oeis.org/A184778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that 2*k + floor(k*sqrt(2)) is prime, listed in
 *            increasing order of k (OEIS offset 1)
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 1944, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;        /* stored k values (as seq_t)         */
    static size_t capacity = 0;        /* current allocation capacity       */
    static size_t count = 0;           /* number of terms currently stored  */
    static idx_t last_k = 0;           /* largest k that has been examined  */

    /* initialise storage */
    if (cache == NULL) {
        capacity = 1024;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (count < n) {
        idx_t k = last_k + 1;          /* next k to test */
        last_k = k;

        seq_t kk = (seq_t)k;           /* k as seq_t */
        seq_t sq = kk * kk;            /* k^2          */
        sq = sq * 2;                   /* 2*k^2        */

        seq_t floork = floor_sqrt(sq); /* floor(k*sqrt(2)) == floor(sqrt(2*k^2)) */
        seq_t expr  = (seq_t)2 * kk + floork; /* 2*k + floor(k*sqrt(2)) */

        if (is_prime_big(expr)) {
            /* grow cache if necessary */
            if (count == capacity) {
                size_t new_cap = capacity * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                capacity = new_cap;
            }
            cache[count++] = kk;        /* store the qualifying k */
        }
    }

    return cache[n-1];   /* a(1) is stored at index 0 */
}
