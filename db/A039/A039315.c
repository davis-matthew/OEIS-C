/*
 * A039315  Numbers whose base-7 representation has the same nonzero number of
 *          1's and 3's.
 * https://oeis.org/A039315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A039315: numbers whose base-7 representation has the same nonzero number of 1's and 3's. */

#include <stddef.h>
#include <stdlib.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* check whether a number satisfies the property */
static int matches_property(seq_t x) {
    unsigned cnt1 = 0, cnt3 = 0;
    seq_t t = x;
    while (t > 0) {
        unsigned d = (unsigned)(t % 7);
        if (d == 1) ++cnt1;
        else if (d == 3) ++cnt3;
        t /= 7;
    }
    return (cnt1 == cnt3) && (cnt1 > 0);
}

/* static cache of found terms */
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;
static seq_t next_num = 1;

seq_t a(idx_t n) {
    if (n < 1u) return 0;     /* offset is 1; no defined value for n < 1 */
    while ((idx_t)terms_len < n) {
        if (matches_property(next_num)) {
            if (terms_len == terms_cap) {               /* grow cache */
                size_t newcap = terms_cap ? terms_cap * 2 : 16;
                seq_t *newptr = seq_realloc(terms, terms_cap, newcap);
                terms = newptr;
                terms_cap = newcap;
            }
            terms[terms_len++] = next_num;
        }
        ++next_num;                                      /* next number to test */
    }
    return terms[n - 1];
}
