/*
 * A023720  Numbers with exactly 3 3's in base 4 expansion.
 * https://oeis.org/A023720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers with exactly three 3's in their base‑4 expansion
   (OEIS A023720).  The function generates the terms lazily, caching
   results so that calls are O(1) after the first generation. */

#include <stdint.h>
#include <stdlib.h>

/* `seq_t` and `idx_t` are supplied by the build environment.  */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1.  Undefined behaviour
       otherwise – return 0 for safety. */
    if (n < 1) return (seq_t)0;

    /* Static storage for the terms that have been generated so far. */
    static seq_t *terms      = NULL;   /* array of values */
    static size_t terms_len  = 0;      /* number of stored terms   */
    static size_t terms_cap  = 0;      /* allocated capacity      */
    static idx_t  next_num   = 0;      /* next integer to test    */

    /* Ensure we have room for at least n terms.  Grow to a power of two
       for efficiency. */
    if (terms_cap < n) {
        size_t new_cap = (n < 64) ? 64 : n;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        terms  = tmp;
        terms_cap = new_cap;
    }

    /* Generate terms until we have the nth term. */
    while (terms_len < n) {
        seq_t val = (seq_t)next_num;
        /* Count digits equal to 3 in base‑4 representation. */
        idx_t  t   = next_num;
        int    cnt = 0;
        while (t) {
            if ((t & 3) == 3) ++cnt;   /* check least significant base‐4 digit */
            t >>= 2;                   /* move to next digit (dividing by 4) */
            if (cnt > 3) break;        /* early exit if more than 3 */
        }
        if (cnt == 3) {
            terms[terms_len++] = val;
        }
        ++next_num;
    }

    return terms[n - 1];
}
