/*
 * A043776  Numbers k such that number of runs in the base 3 representation of
 *          k is congruent to 0 mod 4.
 * https://oeis.org/A043776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers k whose number of runs in the base‑3 representation
   is congruent to 0 (mod 4).  The sequence is given by
   a(n) = the n‑th such number (offset 1). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for the terms computed so far */
    static seq_t *cache = NULL;          /* array of found terms */
    static size_t cap = 0;              /* current capacity of cache   */
    static idx_t terms = 0;             /* how many terms are stored   */
    static seq_t next = 1;              /* next integer to test        */

    /* enlarge the cache if necessary */
    if (cap < n) {
        size_t newcap = cap ? cap * 2 : 128;
        if (newcap < n) newcap = n;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* generate terms until we have at least n of them */
    while (terms < n) {
        seq_t k = next;
        seq_t temp = k;
        int prev = -1;   /* previous digit, -1 means none yet   */
        int runs = 0;    /* number of runs in the base‑3 form   */

        /* convert k to base 3 and count runs */
        while (temp != 0) {
            int digit = (int)(temp % 3);
            temp = temp / 3;
            if (digit != prev) {
                runs++;
                prev = digit;
            }
        }

        /* record the term if it satisfies the condition */
        if (runs % 4 == 0) {
            cache[terms++] = k;
        }

        next++;
    }

    return cache[n - 1];
}
