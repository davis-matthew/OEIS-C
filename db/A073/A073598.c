/*
 * A073598  Numbers n such that n^3 + 5 is prime.
 * https://oeis.org/A073598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 362, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for the nth integer n such that n³ + 5 is prime. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Static cache of found values */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static size_t  len   = 0;

    if (n == 0)
        return 0; /* undefined offset; guard against accidental misuse */

    /* Starting point for the search: one after the last stored value
       (or 1 if none stored yet). */
    idx_t cur_n = (len == 0) ? 1 : (idx_t)(cache[len - 1] + 1);

    while (len < n) {
        seq_t sn   = (seq_t)cur_n;                 /* current candidate */
        seq_t cube = sn * sn * sn + (seq_t)5;       /* n³ + 5 */

        if (isPrimeGeneric(cube)) {                /* test primality */
            /* Grow cache if needed */
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 4 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = sn;                      /* store qualifying n */
        }
        cur_n++;  /* next candidate integer */
    }

    return cache[n - 1]; /* zero‑based indexing */
}
