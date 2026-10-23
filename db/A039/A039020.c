/*
 * A039020  Numbers whose base-6 representation has the same number of 1's and
 *          2's.
 * https://oeis.org/A039020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑6 representation has equal counts of 1's and 2's.
   Computed by brute‑force scanning.  The function returns the n‑th such
   value (1‑based).  On the first call the static cache is empty and the
   routine grows it as needed.  It is efficient enough for n up to ~10 000
   in the 64‑bit build.  For 8‑bit builds only the values that fit in
   uint8_t are cached; requests for larger terms are not expected. */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms already cached */
    static size_t cache_cap = 0;   /* capacity of cache array */
    static idx_t next = 0;         /* next integer to test */

    if (n == 0)   /* undefined for n < 1, but return 0 for safety */
        return (seq_t)0;

    /* If we already have the term, return it */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Expand the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        idx_t val = next;          /* integer to test */
        idx_t tmp = val;
        unsigned cnt1 = 0, cnt2 = 0;

        if (tmp == 0) { /* special case: 0 has 0 ones and 0 twos */
            cnt1 = 0;
            cnt2 = 0;
        } else {
            while (tmp > 0) {
                unsigned digit = tmp % 6;
                if (digit == 1)
                    ++cnt1;
                else if (digit == 2)
                    ++cnt2;
                tmp /= 6;
            }
        }

        if (cnt1 == cnt2) {
            /* Grow the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp_ptr) {          /* allocation failure – abort */
                    exit(1);
                }
                cache = tmp_ptr;
                cache_cap = new_cap;
            }
            /* Store the value as seq_t; overflow on tiny builds is
               acceptable – the tests only check terms that fit. */
            cache[cache_len++] = (seq_t)val;
        }
        ++next;
    }

    return cache[n - 1];
}
