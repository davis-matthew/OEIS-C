/*
 * A227783  Stufe of Q(sqrt(-k)), where k is the n-th squarefree number.
 * https://oeis.org/A227783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A227782(A005117(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, return the “Stufe” of Q(√−k) where k is the n‑th
   square‑free integer.  For k = 1 the level is 1; for k ≠ 1 it is
   4 when k ≡ 7 (mod 8) and 2 otherwise. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* maintain a cached list of square‑free numbers */
    static uint32_t *sf = NULL;      /* array of square‑free integers */
    static size_t  sf_size = 0;      /* how many have been stored */
    static size_t  sf_cap  = 0;      /* current allocation size */
    static uint32_t next    = 1;     /* next integer to test */

    if (n == 0)
        return (seq_t)0;                  /* undefined, guard */

    /* grow the cache until it contains at least n entries */
    while (sf_size < (size_t)n) {
        bool squarefree = true;
        /* trial division up to sqrt(candidate) */
        for (uint32_t p = 2; (uint64_t)p * p <= next; ++p) {
            if (next % (p * p) == 0) {   /* divisible by p² → not square‑free */
                squarefree = false;
                break;
            }
        }

        if (squarefree) {
            if (sf_size >= sf_cap) {      /* enlarge array */
                size_t new_cap = sf_cap ? sf_cap * 2 : 128;
                uint32_t *tmp = (uint32_t *)realloc(sf,
                                 new_cap * sizeof(uint32_t));
                if (tmp == NULL) {
                    /* allocation failed – return zero as fallback */
                    return (seq_t)0;
                }
                sf    = tmp;
                sf_cap = new_cap;
            }
            sf[sf_size++] = next;
        }
        ++next;
    }

    /* the n‑th square‑free number (1‑based) */
    uint32_t k = sf[n - 1];

    /* compute the level (Stufe) of Q(√−k) */
    if (k == 1u)
        return (seq_t)1;
    else if ((k & 7U) == 7U)
        return (seq_t)4;
    else
        return (seq_t)2;
}
