/*
 * A026580  T(n,0) + T(n,1) + ... + T(n,n), T given by A026568.
 * https://oeis.org/A026580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   reallocate_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence (OEIS A026580):
      a(0)=1, a(1)=2, a(2)=5, a(3)=12, a(4)=30, a(5)=75, a(6)=189.
      For n≥7 :
        n*a(n)+(n-2)*a(n-1)+(-17*n+28)*a(n-2)+5*(-n+4)*a(n-3)
        +2*(34*n-107)*a(n-4)+16*(n-5)*a(n-5)+32*(-2*n+9)*a(n-6)=0.
      The division by n is exact for all n≥7. */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* uint64_t */
#include "../utils/reallocate_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  up_to = (idx_t)-1;   /* last index computed */

    /* First use: initialise cache with the first 7 values */
    if (cache == NULL) {
        const seq_t init[7] = {1, 2, 5, 12, 30, 75, 189};
        cap   = 7;
        cache = reallocate_seq(NULL, 0, cap);
        if (cache == NULL) return 0;    /* allocation failure */
        for (size_t i = 0; i < 7; ++i) cache[i] = init[i];
        up_to = 6;
    }

    /* Return early for small n (≤ up_to) */
    if (n <= up_to) return cache[(size_t)n];

    /* Grow cache if needed */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t new_cap = cap ? cap : 16;
        while (new_cap < needed) new_cap <<= 1;
        cache = reallocate_seq(cache, cap, new_cap);
        if (cache == NULL) return 0;  /* allocation failure */
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = up_to + 1; i <= n; ++i) {
        /* Positive part: (i-2)*a(i-1) + 2*(34*i-107)*a(i-4) + 16*(i-5)*a(i-5) */
        seq_t part1 = (seq_t)(i - 2) * cache[i - 1];
        seq_t part2 = (seq_t)(2 * (34 * i - 107)) * cache[i - 4];
        seq_t part3 = (seq_t)(16 * (i - 5)) * cache[i - 5];
        seq_t positive = part1 + part2 + part3;

        /* Negative part (expressed as positive contributions):
           -(17*i-28)*a(i-2) -(5*(i-4))*a(i-3) -(32*(2*i-9))*a(i-6) */
        seq_t neg1 = (seq_t)(17 * i - 28) * cache[i - 2];
        seq_t neg2 = (seq_t)(5 * (i - 4)) * cache[i - 3];
        seq_t neg3 = (seq_t)(32 * (2 * i - 9)) * cache[i - 6];
        seq_t negative = neg1 + neg2 + neg3;

        /* From the recurrence: i*a(i) = negative - positive */
        seq_t numerator = negative - positive;
        cache[i] = numerator / (seq_t)i;   /* division exact */
    }

    up_to = n;
    return cache[(size_t)n];
}
