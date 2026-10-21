/*
 * A053315  a(n) contains n digits (either '4' or '5') and is divisible by
 *          2^n.
 * https://oeis.org/A053315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=4; a(n)=a(n-1)+10^(n-1)*(4+(a(n-1)/2^(n-1) mod 2))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow10.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Cache already computed terms */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* number of terms stored (>=1 if cache non‑NULL) */
    static size_t capacity = 0;

    if (n == 0)          /* undefined for n<1, but return 0 safely */
        return 0;

    /* First call – allocate and initialize */
    if (cache == NULL) {
        capacity = 64;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – unlikely */
        cache[0] = (seq_t)4;           /* a(1) */
        cached = 1;
    }

    /* Extend cache if needed */
    if (n > cached) {
        size_t new_cap = capacity;
        while ((idx_t)new_cap < n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure – unlikely */
        cache = tmp;
        capacity = new_cap;

        /* Compute missing terms */
        for (idx_t i = cached; i < n; ++i) {
            /* i indexes cache[i] = a(i+1) */
            seq_t prev = cache[i - 1];          /* a(i)          */
            seq_t pow_two = pow2(i);            /* 2^i = 2^(n-1) */
            seq_t quotient = prev / pow_two;    /* a(i)/2^i     */
            seq_t digit = (seq_t)(4 + (quotient & (seq_t)1));
            seq_t pow10_val = pow10(i);         /* 10^i = 10^(n-1) */
            cache[i] = prev + pow10_val * digit;
        }
        cached = n;
    }

    return cache[n - 1];
}
