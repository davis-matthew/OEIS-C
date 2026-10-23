/*
 * A039376  Numbers whose base-9 representation has the same nonzero number of
 *          2's and 7's.
 * https://oeis.org/A039376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 7662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Search: return the nth integer whose base‑9 expansion contains the
 *  same non‑zero number of 2’s and 7’s.  This function stores found terms
 *  in a static cache so that repeated calls are O(1) after the search. */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#  include <new>
#  include <utility>
#endif

/* Count 2's and 7's in base 9. */
static bool equal_27(seq_t n)
{
    unsigned int cnt2 = 0, cnt7 = 0;
    seq_t temp = n;
    while (temp != 0) {
        int d = (int)(temp % 9);   /* mod 9 gives a value 0–8 */
        if (d == 2) ++cnt2;
        else if (d == 7) ++cnt7;
        temp = temp / 9;
    }
    return (cnt2 > 0) && (cnt2 == cnt7);
}

/* Reallocate an array of seq_t, copying the existing elements. */
#ifdef __cplusplus
static seq_t *realloc_seq(seq_t *old, size_t oldcap, size_t newcap)
{
    seq_t *newptr = new seq_t[newcap];
    for (size_t i = 0; i < oldcap; ++i)
        newptr[i] = std::move(old[i]);
    delete[] old;
    return newptr;
}
#else
static seq_t *realloc_seq(seq_t *old, size_t /*oldcap*/, size_t newcap)
{
    return (seq_t *)realloc(old, newcap * sizeof(seq_t));
}
#endif

/* Cache for computed terms. */
static seq_t *cache = NULL;
static size_t cap    = 0;   /* allocated capacity */
static size_t size   = 0;   /* number of stored terms    */
static seq_t current = 0;   /* last integer examined    */

seq_t a(idx_t n)
{
    if (n == 0) {          /* behaviour below the offset is irrelevant */
        return 0;
    }
    /* Grow the cache until we have at least n terms. */
    while (size < (size_t)n) {
        seq_t candidate = current + 1;

        if (equal_27(candidate)) {
            if (size == cap) {
                size_t newcap = (cap == 0 ? 8 : cap * 2);
                seq_t *newptr = realloc_seq(cache, cap, newcap);
                if (!newptr) abort();
                cache = newptr;
                cap = newcap;
            }
            cache[size++] = candidate;
        }
        current = candidate;
    }

    return cache[n - 1];
}
