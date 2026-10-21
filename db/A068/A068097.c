/*
 * A068097  a(1) = 1; a(n+1) = smallest triangular numbers starting with a(n).
 * https://oeis.org/A068097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A068097:
   a(1)=1;
   a(n+1)=smallest triangular number that has a(n) as a decimal prefix.
   The implementation follows the definition by brute‑force search:
   for each exponent E≥1 it looks for the first triangular number in
   [ a(n)*10^E , (a(n)+1)*10^E ). The sequence is cached so that
   calls may come in any order.                     */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/pow10.h"        /* seq_t pow10(idx_t e)            */
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x)       */

static seq_t next_triangular_start(seq_t prev)
{
    /* E starts at 1, i.e. 10^1 = 10. */
    seq_t pow10e = pow10(1);

    for (;; pow10e = pow10e * (seq_t)10) {
        /* bounds for the interval of prefixes of prev */
        seq_t lower = prev * pow10e;
        seq_t upper = lower + pow10e;   /* (prev+1)*10^E */

        /* Solve k(k+1)/2 >= lower  ->  k ≈ floor((sqrt(1+8*lower)-1)/2)   */
        seq_t tmp = lower;
        tmp = tmp * (seq_t)8;
        tmp = tmp + (seq_t)1;
        seq_t d = floor_sqrt(tmp);
        seq_t k = (d - (seq_t)1) / (seq_t)2;
        seq_t t = k * (k + (seq_t)1) / (seq_t)2;

        /* Ensure t >= lower       */
        while (t < lower) {
            ++k;
            t = k * (k + (seq_t)1) / (seq_t)2;
        }

        /* If it lies before upper, we found the smallest triangular number
           beginning with prev.                                      */
        if (t < upper) {
            return t;
        }
        /* otherwise increase E and try again */
    }
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* cached values   */
    static size_t capacity = 0;   /* allocated size  */
    static size_t size = 0;       /* number of terms stored */

    if (n == 0) { /* undefined for n=0 */
        return (seq_t)0;
    }

    /* grow array if necessary */
    if ((size_t)n > size) {
        size_t need = (size_t)n;
        if (need > capacity) {
            size_t new_cap = (capacity > 0 ? capacity * 2 : 1);
            if (new_cap < need) new_cap = need;
            seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!new_terms) abort();
            terms = new_terms;
            capacity = new_cap;
        }
        /* initialise when empty */
        if (size == 0) {
            terms[0] = (seq_t)1;    /* a(1)=1 */
            size = 1;
        }
        /* compute remaining terms up to n */
        for (; size < need; ++size) {
            terms[size] = next_triangular_start(terms[size - 1]);
        }
    }

    return terms[n - 1];
}
