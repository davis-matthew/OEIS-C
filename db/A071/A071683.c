/*
 * A071683  Nonprimes which are the average of two consecutive Fibonacci
 *          numbers.
 * https://oeis.org/A071683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Generate consecutive Fibonacci numbers.  
   For each pair (F_k,F_{k+1}) compute the integer average  
   (F_k+F_{k+1})/2.  If the average is even and not prime it is  
   stored as the next term of the sequence.  a(1)=1 is the first  
   such non‑prime average. */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */

seq_t a(idx_t n)
{
    /* Static cache of already found terms (1‑based index).  */
    static seq_t *cache   = NULL;   /* array of stored values   */
    static size_t cap     = 0;      /* allocated capacity      */
    static size_t used    = 0;      /* number of cached terms   */

    /* Current consecutive Fibonacci pair F_k, F_{k+1}.  */
    static seq_t f = 0, g = 0;      /* f = F_k, g = F_{k+1}    */

    /* Initialise the first Fibonacci pair on the first call. */
    if (used == 0) {
        f = (seq_t)1;  /* F_1 */
        g = (seq_t)1;  /* F_2 */
    }

    /* Extend the cache until it contains at least n terms. */
    while (used < n) {
        seq_t sum = f + g;                 /* F_k + F_{k+1}            */

        /* Keep only even sums, which give integer averages. */
        if ((sum & ((seq_t)1)) == 0) {
            seq_t avg = sum / ((seq_t)2);  /* (F_k + F_{k+1}) / 2      */

            /* Store non‑prime averages (1 is considered nonprime). */
            if (!is_prime_big(avg)) {
                if (used >= cap) {
                    size_t new_cap = cap ? cap << 1 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                   new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cap   = new_cap;
                }
                cache[used++] = avg;
            }
        }

        /* Advance to the next consecutive Fibonacci numbers. */
        seq_t new_f = g;          /* F_{k+1} becomes new F_k  */
        seq_t new_g = f + g;      /* F_{k+2} = F_k + F_{k+1}  */
        f = new_f;
        g = new_g;
    }

    return cache[(size_t)(n - (idx_t)1)];
}
