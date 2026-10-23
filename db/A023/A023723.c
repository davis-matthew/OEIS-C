/*
 * A023723  Numbers with exactly 2 0's in their base 5 expansion.
 * https://oeis.org/A023723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Numbers with exactly 2 0's in their base‑5 expansion (computed
 *            by searching)
 * Range:     u8 n <= 21, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of sequence values found so far. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t sz     = 0;   /* number of entries stored in cache   */
    static idx_t tested = 0;   /* number being tested (next value)    */

    /* Generate terms until we have at least n of them. */
    while (sz < n) {
        seq_t cur = (seq_t)tested;   /* candidate number */

        /* Count zeros in the base‑5 representation of cur. */
        int zeros = 0;
        seq_t temp = cur;
        while (temp > 0) {
            if ((temp % 5) == 0) zeros++;
            temp /= 5;
        }

        /* Store if it has exactly two zeros. */
        if (zeros == 2) {
            if (sz == cap) {
                size_t new_cap = cap ? 2 * cap : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1); /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[sz++] = cur;
        }

        tested++;
    }

    return cache[n - 1];
}
