/*
 * A009995  Numbers with digits in strictly decreasing order. From the
 *          Macaulay expansion of n.
 * https://oeis.org/A009995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base, look
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 55, u32 n <= 1022, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with strictly decreasing decimal digits (0..9).
   a(n) is the n‑th smallest such number, with a(1)=0.
   The set of all numbers is the set of all non‑empty subsets of {0..9}
   written with their digits in decreasing order.
   Implementation builds this set once, sorts it ascendingly,
   and returns the requested element. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    const size_t TOTAL  = (1u << 10) - 1;   /* 2^10 – 1 = 1023 subsets */

    /* Build the cache on first invocation */
    if (cache == NULL) {
        cap = TOTAL;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache)                 /* allocation failure – return 0 */
            return 0;

        /* Create each number from a mask of digits. */
        for (idx_t mask = 1; mask <= TOTAL; ++mask) {
            seq_t v = 0;
            for (int d = 9; d >= 0; --d) {
                if (mask & ((idx_t)1 << d))
                    v = v * 10 + (seq_t)d;
            }
            cache[mask - 1] = v;
        }

        /* Simple insertion sort – 1023 elements, negligible cost. */
        for (size_t i = 1; i < cap; ++i) {
            seq_t key = cache[i];
            size_t j = i;
            while (j > 0 && cache[j - 1] > key) {
                cache[j] = cache[j - 1];
                --j;
            }
            cache[j] = key;
        }
    }

    /* Bounds check – the sequence has exactly 1023 terms */
    if (n < 1 || (size_t)(n - 1) >= cap)
        return 0;

    return cache[n - 1];
}
