/*
 * A061354  Numerator of Sum_{k=0..n} 1/k!.
 * https://oeis.org/A061354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of sum_{k=0..n} 1/k! (reduced fraction)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(0)=1, fact(0)=1
   For i>0:
       fact  = fact * i          (i!  )
       T     = T * i + 1         (sum_{k=0..i} i!/k!)
       g     = gcd(T, fact)     (reduce)
       a(i)  = T / g             (numerator)
   This computes a(n) for any n ≥ 0 with O(n) per new maximum. */

#include <stdlib.h>            /* malloc, realloc */
#include "../utils/gcd.h"      /* gcd(seq_t, seq_t) */

static seq_t *A = NULL;        /* cache of numerators a(i) */
static idx_t  max_n = 0;       /* highest n computed */
static seq_t  fact = 1;        /* factorial of max_n (max_n!) */
static seq_t  T    = 1;        /* sum_{k=0..max_n} max_n! / k! */

/* Extend cache up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (A == NULL) {
        size_t sz = (size_t)(n + 1);
        A = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!A) return;                /* allocation failed – caller will get 0 */
        A[0]  = (seq_t)1;               /* a(0) = 1 */
        fact  = (seq_t)1;               /* 0! = 1   */
        T     = (seq_t)1;               /* T(0) = 1  */
        max_n = 0;
        /* If n==0 we are finished. */
    }

    if (n > max_n) {
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(A, new_sz * sizeof(seq_t));
        if (!tmp) return;                /* realloc failed – caller will get 0 */
        A = tmp;

        for (idx_t i = max_n + 1; i <= n; ++i) {
            fact *= (seq_t)i;            /* fact = i!          */
            T    = T * (seq_t)i + 1;     /* T(i) = i*T(i-1)+1  */
            seq_t g = gcd(T, fact);      /* reduce the fraction */
            A[i] = T / g;                /* store numerator     */
        }
        max_n = n;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n <= max_n) return A[n];
    return (seq_t)0;                    /* should never happen */
}
