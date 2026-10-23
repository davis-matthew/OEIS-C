/*
 * A110273  a(n) = Pell(n)^3 + Pell(n+1)^3.
 * https://oeis.org/A110273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Pell(n)^3 + Pell(n+1)^3, initial values: a(0) = 1, a(1) =
 *            9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   cube
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/cube.h"

/* Sequence values are stored in a static cache of Pell numbers.
   Pell(0) = 0, Pell(1) = 1, Pell(k) = 2*Pell(k-1) + Pell(k-2). */
seq_t a(idx_t n)
{
    static seq_t *pell = NULL;   /* cache of Pell numbers */
    static size_t   p_len = 0;   /* number of elements stored (>=1) */

    /* Determine how many Pell values are needed (0…n+1 inclusive). */
    idx_t   need  = n + 1;                  /* last index required       */
    size_t required = (size_t)(need + 1);   /* array length needed       */

    /* Allocate or grow the cache when necessary. */
    if (pell == NULL) {
        pell = (seq_t *)malloc(required * sizeof(seq_t));
        if (pell == NULL) return (seq_t)0;  /* allocation failure */
        pell[0] = (seq_t)0;
        if (required > 1) pell[1] = (seq_t)1;
        p_len = required > 1 ? 2 : 1;
    }
    else if (p_len <= need) {
        size_t new_len = required;
        seq_t *tmp = (seq_t *)realloc(pell, new_len * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        pell = tmp;

        /* initialise missing base cases if the array was empty. */
        if (p_len == 0) {
            pell[0] = (seq_t)0;
            if (new_len > 1) pell[1] = (seq_t)1;
            p_len = new_len > 1 ? 2 : 1;
        }
        else if (p_len == 1) {   /* only Pell(0) present */
            pell[1] = (seq_t)1;
            p_len  = 2;
        }

        /* Compute new Pell numbers on top of the cache. */
        for (idx_t i = (idx_t)p_len; i <= need; ++i) {
            pell[i] = (seq_t)2 * pell[i-1] + pell[i-2];
        }
        p_len = new_len;
    }

    /* Compute the requested term: Pell(n)^3 + Pell(n+1)^3. */
    seq_t pn  = pell[n];
    seq_t pn1 = pell[n+1];
    return cube(pn) + cube(pn1);
}
