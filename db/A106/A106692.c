/*
 * A106692  Numbers k such that 97*k - 11 is prime.
 * https://oeis.org/A106692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   my_is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code: returns the nth integer k such that 97*k-11 is prime. */
/* The function generates numbers on demand and caches results. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/my_is_prime.h"   /* provides my_is_prime(uint64_t) */

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for offset starting at 1 */
        return 0;

    /* static cache of found k's */
    static seq_t *vals = NULL;
    static idx_t sz = 0;
    static idx_t cap = 0;
    static seq_t next_k = 0;

    /* expand cache until we have at least n terms */
    while (sz < n) {
        next_k++;                           /* next candidate k */
        seq_t p = (seq_t)97 * next_k - 11;  /* compute 97*k-11 */
        if (my_is_prime((uint64_t)p)) {     /* test primality */
            /* ensure capacity */
            if (sz == cap) {
                idx_t new_cap = (cap ? cap << 1 : 16);
                vals = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
                cap = new_cap;
            }
            vals[sz++] = next_k;            /* store found k */
        }
    }

    return vals[n - 1];
}
