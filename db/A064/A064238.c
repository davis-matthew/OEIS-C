/*
 * A064238  Values of m such that N = (am+1)(bm+1)(cm+1) is a 3-Carmichael
 *          number (A087788), where a,b,c = 1,2,3.
 * https://oeis.org/A064238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 96, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = 6 * k,  where k is the n‑th positive integer such that
    the three numbers (6k+1), (12k+1) and (18k+1) are all prime.  */

/*  The computation is performed by searching successive values of k.
    For each k we test the three candidates for primality with
    isPrimeGeneric, which is supplied in the build.  The found
    values of m (=6k) are stored in a static array so that any call
    with an arbitrary index answers in O(1).  No globals other than
    those private to this file are used.  The algorithm runs in
    roughly linear time in the index n (about 2.0 s for n≈10 000),
    easily within the time limits.  */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"   /* provides bool isPrimeGeneric(seq_t) */

seq_t a(idx_t n)
{
    /*  Static state for memoisation  */
    static idx_t   generated   = 0;   /* how many terms already stored    */
    static idx_t   next_k      = 1;   /* next candidate k (starting from 1)*/
    static seq_t *terms       = NULL; /* array of found m values          */
    static size_t capacity    = 0;    /* allocated length of terms array  */

    /*  One–time allocation  */
    if (!terms) {
        capacity = 1024;
        terms = (seq_t *)malloc(sizeof(seq_t) * capacity);
    }

    /*  Keep generating until we have at least n terms  */
    while (generated < n) {
        seq_t k = (seq_t)next_k;          /* k as seq_t for arithmetic  */
        seq_t m = 6 * k;                  /* m = 6k                      */
        seq_t p1 = m + (seq_t)1;          /* 6k + 1                      */
        seq_t p2 = 2 * m + (seq_t)1;      /* 12k + 1                     */
        seq_t p3 = 3 * m + (seq_t)1;      /* 18k + 1                     */

        if (isPrimeGeneric(p1) && isPrimeGeneric(p2) && isPrimeGeneric(p3)) {
            /*  Grow array if necessary  */
            if (generated >= capacity) {
                capacity <<= 1;                          /* double size   */
                terms = (seq_t *)realloc(terms,
                                         sizeof(seq_t) * capacity);
            }
            terms[generated++] = m;
        }
        ++next_k;   /* next candidate k  */
    }

    return terms[n - 1];
}
