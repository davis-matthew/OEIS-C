/*
 * A029315  Expansion of 1/((1-x^3)*(1-x^7)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-3)+a(n-7)-a(n-10)+a(n-11)+a(n-12)-a(n-14)-a(n-15)-a(n-18)-a(n-19)+a(n-21)+a(n-22)-a(n-23)+a(n-26)+a(n-30)-a(n-33),
 *            with a(0)=1, a(1)=0, a(2)=0, and a(k)=0 for k<0
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;

    size_t idx = (size_t)n;

    if (idx >= cap) {
        size_t new_cap = idx + 1;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr) return 0; /* allocation failure – unlikely in tests */
        cache = new_arr;

        if (cap == 0) {
            /* Base cases */
            cache[0] = (seq_t)1;
            if (new_cap > 1) cache[1] = (seq_t)0;
            if (new_cap > 2) cache[2] = (seq_t)0;
            cap = 3;
        }

        for (size_t i = cap; i <= idx; ++i) {
            seq_t pos = (seq_t)0;
            seq_t neg = (seq_t)0;

            if (i >= 3)  pos += cache[i-3];
            if (i >= 7)  pos += cache[i-7];
            if (i >= 11) pos += cache[i-11];
            if (i >= 12) pos += cache[i-12];
            if (i >= 21) pos += cache[i-21];
            if (i >= 22) pos += cache[i-22];
            if (i >= 26) pos += cache[i-26];
            if (i >= 30) pos += cache[i-30];

            if (i >= 10) neg += cache[i-10];
            if (i >= 14) neg += cache[i-14];
            if (i >= 15) neg += cache[i-15];
            if (i >= 18) neg += cache[i-18];
            if (i >= 19) neg += cache[i-19];
            if (i >= 23) neg += cache[i-23];
            if (i >= 33) neg += cache[i-33];

            cache[i] = pos - neg; /* pos ≥ neg holds for all n */
        }
        cap = new_cap;
    }

    return cache[idx];
}
