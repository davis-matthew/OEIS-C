/*
 * A037383  Numbers k such that every base-3 digit of k is a base-7 digit of
 *          k.
 * https://oeis.org/A037383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   base3_in_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth positive integer k such that every base‑3 digit of k is a
   digit in the base‑7 representation of k.
   The function builds the sequence by iterating integers, testing the
   condition, and caching previously found terms. */

#include <stdlib.h>
#include "../utils/base3_in_base7.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;
    static seq_t last_checked = 0;

    /* return cached term if already computed */
    if (size >= (size_t)n)
        return cache[n - 1];

    /* initialise cache */
    if (!cache) {
#ifdef __cplusplus
        cache = new seq_t[4];
#else
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
#endif
        cap = 4;
    }

    /* generate terms until we have at least n */
    while (size < (size_t)n) {
        last_checked = last_checked + 1;   /* candidate integer */

        if (base3_in_base7(last_checked)) {
            /* grow cache if necessary */
            if (size == cap) {
                size_t new_cap = cap * 2;
                seq_t *tmp;
#ifdef __cplusplus
                tmp = new seq_t[new_cap];
                for (size_t i = 0; i < size; ++i)
                    tmp[i] = cache[i];
                delete[] cache;
#else
                tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
#endif
                cache = tmp;
                cap = new_cap;
            }
            cache[size++] = last_checked;
        }
    }
    return cache[n - 1];
}
