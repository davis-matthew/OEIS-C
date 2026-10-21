/*
 * A080668  Numbers of the form n!+n^3.
 * https://oeis.org/A080668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! + n^3
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of factorials */
    static seq_t *fact = NULL;
    static size_t   cap = 0;
    static idx_t    len = 0;  /* number of factorials stored, fact[i] = i!  (i from 0) */

    if (!fact) {
        /* initialise cache with room for 64 factorials and set 0! = 1 */
        cap = 64;
        fact = (seq_t*)malloc(cap * sizeof(seq_t));
        len = 1;
        fact[0] = (seq_t)1;          /* 0! = 1 */
    }

    /* grow cache and compute missing factorials up to n */
    while (len <= n) {
        idx_t i = len;                 /* next index to compute */
        seq_t val = fact[i - 1] * (seq_t)i;   /* i! = (i-1)! * i */

        if (len >= cap) {              /* expand capacity */
            cap *= 2;
            fact = (seq_t*)realloc(fact, cap * sizeof(seq_t));
        }
        fact[i] = val;
        ++len;
    }

    /* compute n^3 */
    seq_t nseq = (seq_t)n;
    seq_t n2 = nseq * nseq;
    seq_t n3 = n2 * nseq;

    /* return a(n) = n! + n^3 */
    return fact[n] + n3;
}
