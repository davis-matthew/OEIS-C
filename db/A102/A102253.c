/*
 * A102253  Begin with 3, multiply each digit by 2.
 * https://oeis.org/A102253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(0)=3; for n≥0, a(n+1) is produced by writing 2*d for each
 *            decimal digit d of a(n) in the order they appear
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   resize_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes a(n) for the OEIS sequence A102253. It memoises
   already computed terms using a static dynamically resized array. */

/* The sequence is defined by repeatedly expanding each term's digits,
   multiplying each digit by 2.  For example, starting 3 → 6 → 12 → 2,4 → 4,8 → 8,16 → 2,12 etc. */

#include "../utils/resize_array.h"

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements stored */
    static idx_t next_to_expand = 0;   /* index of the next term whose digits
                                          have yet to be expanded */

    /* grow cache for the first time: a(0)=3 */
    if (cache_size == 0) {
        cache = resize_array(cache, 0, 1);
        cache[0] = (seq_t)3;
        cache_size = 1;
    }

    /* expand terms until we have at least n+1 elements */
    while (cache_size <= n) {
        /* get the current term to expand */
        seq_t value = cache[next_to_expand];
        ++next_to_expand;

        /* extract decimal digits of value in reverse order */
        seq_t tmp = value;
        int digits[64];
        int dcnt = 0;
        if (tmp == 0) {
            digits[dcnt++] = 0;
        } else {
            while (tmp > 0) {
                digits[dcnt++] = (int)(tmp % 10);
                tmp /= 10;
            }
        }

        /* append new terms by doubling each digit, in original order */
        for (int i = dcnt - 1; i >= 0; --i) {
            seq_t d = (seq_t)digits[i];
            seq_t next = d * (seq_t)2;
            cache = resize_array(cache, cache_size, cache_size + 1);
            cache[cache_size++] = next;
        }
    }

    return cache[n];
}
