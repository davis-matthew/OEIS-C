/*
 * A051004  Numbers divisible both by their individual digits and by the sum
 *          of their digits.
 * https://oeis.org/A051004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, base
 * Formula:   a(n) = the nth natural number divisible by each of its non‑zero
 *            digits and by the sum of its digits
 * Range:     u8 n <= 23, u16 n <= 617, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of found terms and state variables for incremental search. */
    static seq_t *cache      = NULL;   /* array of terms              */
    static size_t   size    = 0;       /* number of stored terms      */
    static size_t   capacity = 0;      /* allocated capacity          */
    static seq_t   current   = 1;      /* next integer to test        */

    if (n == 0) return 0;             /* undefined for offset 1      */

    /* Ensure the cache can hold the requested index. */
    while (size < (size_t)n) {
        seq_t val = current;
        seq_t tmp = val;
        seq_t sum = 0;
        int ok   = 1;

        /* Scan digits of val. */
        while (tmp > 0) {
            unsigned d = (unsigned)(tmp % 10);
            if (d == 0) { ok = 0; break; }                   /* zero digit – reject */
            if (val % (seq_t)d != 0) { ok = 0; break; }      /* not divisible by digit */
            sum += d;
            tmp /= 10;
        }

        if (ok && val % sum == 0) {                           /* satisfies both conditions */
            if (size == capacity) {                           /* grow cache if needed */
                size_t new_cap = capacity ? capacity * 2 : 4 ;
                seq_t *tmp_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp_ptr) return 0;                       /* allocation failure → 0 */
                cache    = tmp_ptr;
                capacity = new_cap;
            }
            cache[size++] = val;
        }
        ++current;  /* advance to next integer to test */
    }

    return cache[(size_t)n - 1];
}
