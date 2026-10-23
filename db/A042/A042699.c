/*
 * A042699  Denominators of continued fraction convergents to sqrt(879).
 * https://oeis.org/A042699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A042699: Denominators of continued fraction convergents to sqrt(879).
   The terms satisfy the linear recurrence
     a(n) = 214490648*a(n-18) - a(n-36)    (n > 35)
   The sequence is generated here by simulating the continued‑fraction
   expansion of sqrt(879), storing the denominators q_n of the convergents.
*/
#include <stdint.h>
#include <stdlib.h>

/* Sequence function */
seq_t a(idx_t n)
{
    /* static state for incremental generation */
    static seq_t *cache = NULL;          /* computed denominator values   */
    static size_t  cap   = 0;            /* current allocation size       */
    static idx_t   len   = 0;            /* number of values in cache    */

    /* continued‑fraction state (for sqrt(879)) */
    static idx_t m      = 0;   /* current m in algorithm            */
    static idx_t d      = 1;   /* current d in algorithm            */
    static idx_t acoeff = 29;  /* current partial quotient a_k      */
    const idx_t  a0      = 29;  /* floor(sqrt(879))                 */

    /* previous two denominators needed for the recurrence */
    static seq_t q_prevprev = (seq_t)1; /* q_{-2} */
    static seq_t q_prev     = (seq_t)0; /* q_{-1} */

    /* initialise cache on first call */
    if (!cache) {
        cap = 64;                          /* starting capacity */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;       /* allocation failed */
    }

    /* compute all terms up to n inclusively */
    while (len <= n) {
        /* compute next denominator q_k */
        idx_t a = acoeff;                          /* current partial quote a_k */
        seq_t q = (seq_t)a * q_prev + q_prevprev;  /* q_k = a_k*q_{k-1} + q_{k-2} */

        /* store in cache, growing if required */
        if (len >= cap) {
            size_t new_cap = cap * 2;
            if (new_cap == 0) new_cap = 64;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;  /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }
        cache[len] = q;
        len++;

        /* shift denominator history for next step */
        q_prevprev = q_prev;
        q_prev     = q;

        /* compute next partial quotient using the continued‑fraction
           arithmetic for sqrt(879).  The standard recurrence is:
                m' = d*a - m
                d' = (D - m'^2) / d
                a' = (a0 + m') / d'   (integer division)
          where D = 879, a0 = floor(sqrt(D)). */
        idx_t next_m = d * acoeff - m;
        idx_t next_d = (879 - next_m * next_m) / d;
        idx_t next_a = (a0 + next_m) / next_d;

        m      = next_m;
        d      = next_d;
        acoeff = next_a;
    }

    return cache[n];
}
