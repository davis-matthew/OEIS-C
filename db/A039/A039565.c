/*
 * A039565  Numbers whose base-5 representation has the same number of 0's,
 *          1's and 4's.
 * https://oeis.org/A039565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = numbers whose base-5 representation has equal numbers of
 *            0, 1 and 4
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;
    static idx_t  next  = 1;

    if (n == 0) return 0;  /* safety for non-positive indices */

    /* generate terms until we have at least n of them */
    while (sz < (size_t)n) {
        idx_t v = next;
        idx_t c0 = 0, c1 = 0, c4 = 0;
        idx_t t = v;

        while (t > 0) {
            idx_t d = t % 5;
            if (d == 0)   c0++;
            else if (d == 1) c1++;
            else if (d == 4) c4++;
            t /= 5;
        }

        if (c0 == c1 && c1 == c4) {
            if (sz == cap) {
                size_t newcap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp;
                if (cap == 0)
                    tmp = seq_alloc(newcap);
                else
                    tmp = seq_realloc(cache, cap, newcap);
                if (!tmp) abort(); /* allocation failure */
                cache = tmp;
                cap   = newcap;
            }
            cache[sz] = (seq_t)v;
            sz++;
        }
        next++;   /* move to next integer to test */
    }

    return cache[n - 1];
}
