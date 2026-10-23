/*
 * A117183  a(n) = smallest prime dividing n-th nonsquarefree positive
 *          integer.
 * https://oeis.org/A117183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime dividing the n-th nonsquarefree positive
 *            integer
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The function a(n) assumes seq_t and idx_t are defined by the build
   system as unsigned integer types (uint8_t … uint64_t or big integer). */
seq_t a(idx_t n)
{
    /* static state: an array of the first nslen nonsquarefree integers
       encountered so far, and the next integer to test. */
    static idx_t *nslist = NULL;
    static size_t nslen  = 0;   /* number of entries stored */
    static size_t nscap  = 0;   /* capacity of nslist */
    static idx_t  cand   = 1;   /* next integer to test */

    if (n == 0) return 0;          /* undefined for n < 1 */

    /* Allocate or grow the storage if we have not yet cached n elements. */
    if (nscap < (size_t)n)
    {
        size_t newcap = n * 2;               /* conservative growth */
        idx_t *tmp = (idx_t *)realloc(nslist, newcap * sizeof(idx_t));
        if (!tmp) return 0;                  /* allocation failure */
        nslist = tmp;
        nscap  = newcap;
    }

    /* Enumerate numbers until we have at least n nonsquarefree integers. */
    while (nslen < (size_t)n)
    {
        ++cand;
        idx_t m = cand;
        bool hasSquareFactor = false;

        /* Check whether there exists a prime p with p² | m. */
        for (idx_t p = 2; ; ++p)
        {
            if (p > m / p) break;   /* p² > m → stop */
            idx_t p2 = p * p;
            if (m % p2 == 0) { hasSquareFactor = true; break; }
        }

        if (hasSquareFactor)
            nslist[nslen++] = m;    /* store the nonsquarefree number */
    }

    /* Now compute the smallest prime divisor of the nth nonsquarefree number. */
    idx_t val = nslist[n - 1];
    idx_t spf = val;   /* fallback if val is prime (unlikely for nonsquarefree) */

    /* Trial division to find the smallest divisor. */
    for (idx_t d = 2; d <= val / d; ++d)
    {
        if (val % d == 0)
        {
            spf = d;
            break;
        }
    }

    return (seq_t)spf;
}
