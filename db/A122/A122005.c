/*
 * A122005  Triangle read by rows: n-th row starts with n and continues with
 *          1/3 the previous value as long as that is an integer.
 * https://oeis.org/A122005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(1)=1; for n>1: if 3 divides a(n-1) then a(n)=a(n-1)/3, else
 *            a(n)=max_{k<n} a(k)+1
 * Range:     u8 n <= 381, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms, 1-based index */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated size of cache */
    static idx_t last_n = 0;        /* last index that has been computed */
    static seq_t current_max = 0;   /* maximum value seen so far */

    if (n < 1) return 0;  /* offset is 1; values for n<1 are irrelevant */

    /* initialize cache at first call */
    if (cap == 0) {
        cap = 2; /* at least index 1 */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* out of memory */
        cache[1] = (seq_t)1;
        current_max = cache[1];
        last_n = 1;
    }

    /* grow cache if needed */
    if (n >= cap) {
        size_t newcap = cap * 2;
        if (newcap <= n) newcap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0; /* out of memory */
        cache = tmp;
        cap = newcap;
    }

    /* compute missing values up to n */
    while (last_n < n) {
        seq_t prev = cache[last_n];
        /* check divisibility by 3 */
        if ((prev % (seq_t)3) == (seq_t)0) {
            cache[last_n + 1] = prev / (seq_t)3;
        } else {
            cache[last_n + 1] = current_max + (seq_t)1;
        }
        /* update maximum */
        if (cache[last_n + 1] > current_max) {
            current_max = cache[last_n + 1];
        }
        last_n++;
    }

    return cache[n];
}
