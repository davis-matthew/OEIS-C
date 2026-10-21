/*
 * A072193  Irregular triangle read by rows of continued fraction expansions
 *          of the rational numbers 1/2, 1/3, 2/3, 1/4, 2/4, 3/4, 1/5, 2/5,
 *          ... A002260(k) / A003057(k+1).
 * https://oeis.org/A072193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9968, u16 n <= 9968, u32 n <= 9968, u64 n <= 9968,
 *            u128 n <= 9968, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS A072193:
   Generates the continued‑fraction digits of the fractions
   A002260(k)/A003057(k+1) (i.e. 1/2,1/3,2/3,1/4,2/4,3/4,…),
   concatenates them (omitting leading zeros) and returns the n‑th
   element of the resulting sequence (a(1) = the first digit). */

#include <stdlib.h>
#include <stdbool.h>

/* External types supplied by the build system:
   idx_t   – unsigned 64‑bit integer
   seq_t   – unsigned integer (8/16/32/64/128 bits or an arbitrary‑precision class)   */
seq_t a(idx_t n)
{
    /* static cache of generated terms */
    static seq_t *data      = NULL;
    static size_t   cap     = 0;
    static size_t   len     = 0;
    static idx_t    next_k  = 1;   /* next fraction index (k) to generate */

    /* ensure at least n terms are available */
    while (len < (size_t)n) {

        /* ***************************************************************************
           Step 1 – determine the k‑th fraction p/q
        */
        idx_t k = next_k++;                   /* k starts from 1 */
        idx_t q = 2;
        /* find smallest q such that (q‑1)*q/2 ≥ k  */
        while ((q - 1) * q / 2 < k) q++;

        idx_t prev = (q - 2) * (q - 1) / 2;   /* number of fractions before this q */
        idx_t p   = k - prev;                 /* numerator in 1 … q‑1 */

        /* ***************************************************************************
           Step 2 – produce the continued‑fraction digits of p/q
        */
        seq_t a1 = (seq_t)p;
        seq_t b1 = (seq_t)q;
        bool skip_zero = true;                /* omit leading zeros */

        while (b1 != 0) {
            seq_t a_val = a1 / b1;            /* integer part of a1/b1 */
            seq_t r     = a1 % b1;            /* remainder */

            if (!(skip_zero && a_val == 0)) {
                /* append a_val to data[] */
                if (len == cap) {
                    size_t new_cap = (cap == 0) ? 128 : cap * 2;
                    seq_t *tmp = (seq_t *)realloc(data, new_cap * sizeof(seq_t));
                    if (!tmp) abort();        /* out of memory */
                    data = tmp;
                    cap  = new_cap;
                }
                data[len++] = a_val;
            }

            skip_zero = false;
            a1 = b1;
            b1 = r;
        }
    }

    /* return the requested term */
    return data[n - 1];
}
