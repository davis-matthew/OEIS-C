/*
 * A067370  The weight of the periphery of the alternating group, denoted
 *          v(P_N).
 * https://oeis.org/A067370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = ½ * ( (-1)^n * (n-2)! + n * n! - |Stirling1(n+1,2)| ) for
 *            n > 1; a(1) = 0, a(2) = 1. Stirling1(k,2) is the unsigned
 *            Stirling number of the first kind
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* dynamic arrays for factorials and unsigned Stirling numbers of the first
   kind with k = 2 */
static seq_t *fact   = NULL;  /* fact[i] = i! */
static size_t fact_len = 0;   /* current allocated length */

static seq_t *stirr = NULL;  /* stirr[i] = Stirling1(i,2) */
static size_t stirr_len = 0;  /* current allocated length */

/* ensure factorial array contains indices up to n inclusive */
static void ensure_fact(idx_t n)
{
    if (fact_len > (size_t)n) return;
    size_t new_len = (size_t)n + 1;
    /* realloc with cast to support arbitrary-precision seq_t */
    fact = (seq_t *)realloc(fact, new_len * sizeof *fact);
    /* initialise new entries */
    if (fact_len == 0) {
        /* 0! = 1 */
        fact[0] = (seq_t)1;
        fact_len = 1;
    }
    for (size_t i = fact_len; i < new_len; ++i) {
        fact[i] = fact[i-1] * (seq_t)i;
    }
    fact_len = new_len;
}

/* ensure Stirling array contains indices up to n inclusive (k=2)
   requires factorials up to n-2 */
static void ensure_stirr(idx_t n)
{
    if (stirr_len > (size_t)n) return;
    size_t new_len = (size_t)n + 1;
    stirr = (seq_t *)realloc(stirr, new_len * sizeof *stirr);
    if (stirr_len == 0) {
        /* base values:
           s(0,2)=0, s(1,2)=0, s(2,2)=1 */
        stirr[0] = (seq_t)0;
        stirr[1] = (seq_t)0;
        stirr[2] = (seq_t)1;
        stirr_len = 3;
    }
    for (size_t i = stirr_len; i < new_len; ++i) {
        /* s(i,2) = (i-1)*s(i-1,2) + (i-2)!   (unsigned) */
        /* ensure factorial up to i-2 is ready */
        if (i >= 2) ensure_fact((idx_t)(i-2));
        stirr[i] = (seq_t)(i-1) * stirr[i-1] + fact[i-2];
    }
    stirr_len = new_len;
}

/* OEIS sequence A067370 */
seq_t a(idx_t n)
{
    /* offsets */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* factorials up to n */
    ensure_fact(n);
    /* Stirling numbers up to n+1 (k=2) */
    ensure_stirr(n+1);

    /* compute term = n*n! */
    seq_t term = fact[n] * (seq_t)n;

    /* add or subtract (n-2)! according to parity of n */
    if ((n & 1u) == 0) {          /* n even */
        term += fact[n-2];
    } else {                      /* n odd  */
        term -= fact[n-2];
    }

    /* subtract Stirling1(n+1,2) (unsigned) */
    term -= stirr[n+1];

    /* divide by 2 (integer division, result is even) */
    term /= (seq_t)2;

    return term;
}
