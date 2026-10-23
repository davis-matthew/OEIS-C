/*
 * A173249  Partial sums of A000272.
 * https://oeis.org/A173249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{i=0..n} i^(i-2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial sums of the number of trees on n labeled nodes  */
/* Implementation uses a cache that grows with n and big_pow from utils    */
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    /* cache[i] holds a(i) = Σ_{k=0..i} k^(k-2) */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* initialise cache with a(0)=1 */
    if (cache_cap == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        assert(cache != NULL);
        cache[0] = 1;          /* a(0) = 1 */
        cache_cap = 1;
    }

    /* grow cache if needed */
    if (n >= cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        assert(tmp != NULL);
        cache = tmp;
        for (idx_t i = cache_cap; i <= n; ++i) {
            seq_t term;
            if (i <= 2) {
                /* conventionally, 0^(−2),1^(−1),2^0 are taken as 1 for this sequence */
                term = 1;
            } else {
                term = big_pow((seq_t)i, (seq_t)(i - 2));
            }
            cache[i] = cache[i - 1] + term;
        }
        cache_cap = n + 1;
    }

    return cache[n];
}
