/*
 * A039076  Numbers whose base-8 representation has the same number of 6's and
 *          7's.
 * https://oeis.org/A039076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base-8 representation has equal counts of 6 and 7 */
/* This implementation stores found values in a static cache and generates new
   candidates until the requested index is reached. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* cached sequence values          */
    static size_t  cap       = 0;      /* current allocation capacity     */
    static size_t  size      = 0;      /* number of values stored          */
    static seq_t  next_candidate = 0;   /* next integer to test             */

    if (n == 0)
        return (seq_t)0; /* not used, but safe fallback */

    while (size < (size_t)n) {
        seq_t x = next_candidate;
        unsigned c6 = 0, c7 = 0;
        seq_t tmp = x;

        /* Count digits 6 and 7 in base‑8 representation. */
        if (tmp == 0) {
            /* 0 has no digits, so c6==c7==0, qualifies. */
        } else {
            while (tmp > 0) {
                unsigned d = (unsigned)(tmp % 8);
                if (d == 6) ++c6;
                else if (d == 7) ++c7;
                tmp /= 8;
            }
        }

        if (c6 == c7) {          /* this number qualifies */
            if (size == cap) {   /* grow cache if needed */
                size_t new_cap = cap ? cap * 2 : 128;
                seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache = new_cache;
                cap   = new_cap;
            }
            cache[size++] = x;
        }
        ++next_candidate;        /* proceed to next integer */
    }

    return cache[n-1]; /* indices are 1‑based in OEIS */
}
