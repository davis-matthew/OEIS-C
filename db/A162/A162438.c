/*
 * A162438  a(1)=1, a(2)=2. Take terms a(n-1) and a(n-2), then convert to
 *          binary. Concatenate them, with either binary a(n-1) on the left
 *          and a(n-2) on the right, or with a(n-1) on the right and a(n-2) on
 *          the left such that the value of the resulting binary number is
 *          maximized. a(n) = the decimal equivalent...
 * https://oeis.org/A162438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=1, a(2)=2, a(n)=max((a(n-1)<<len(a(n-2)))|a(n-2),
 *            (a(n-2)<<len(a(n-1)))|a(n-1)) for n>2 where len(x) is the number
 *            of bits needed to represent x
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   bitlen_seq_t_dummy
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/bitlen_seq_t_dummy.h"

/* external linkage as required */
seq_t a(idx_t n)
{
    /* 1‑based cache; cache[0] unused */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size */
    static idx_t high   = 0;   /* highest index computed */

    if (n == 0) {
        /* undefined for n=0, but return 0 to avoid UB */
        return 0;
    }

    /* Make sure the cache can hold up to index n */
    if (high < n) {
        size_t needed = (size_t)n + 1;          /* +1 because index 0 unused */
        if (cap < needed) {
            size_t newcap = cap ? cap : 2;
            while (newcap < needed) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cache = tmp;
            cap = newcap;
        }

        /* Compute missing terms */
        for (idx_t i = high + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = 1;
            } else if (i == 2) {
                cache[2] = 2;
            } else {
                seq_t a1 = cache[i - 1];
                seq_t a2 = cache[i - 2];
                unsigned len1 = bitlen_seq_t_dummy(a1);
                unsigned len2 = bitlen_seq_t_dummy(a2);
                seq_t v1 = (a1 << len2) | a2;
                seq_t v2 = (a2 << len1) | a1;
                cache[i] = v1 > v2 ? v1 : v2;
            }
        }
        high = n;
    }
    return cache[n];
}
