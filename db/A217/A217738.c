/*
 * A217738  Numbers k such that Fibonacci(k) is divisible by k*(k+1).
 * https://oeis.org/A217738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 153, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   fib_mod_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A217738 – The n‑th integer k such that Fibonacci(k)
    is divisible by k*(k+1).  The function searches k sequentially,
    testing divisibility via a fast‑doubling Fibonacci modulo
    computation.  It memoises the results so calls in any order are
    efficient.  */

/*  Requires the usual stdint.h, stdlib.h and the type definitions
    seq_t and idx_t supplied by the build environment.  */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/fib_mod_u128.h"

/*---- Main sequence function --------------------------------------------*/
seq_t a(idx_t n)
{
    /* Memoisation tables */
    static seq_t *terms   = NULL;   /* cached k‑values             */
    static size_t  len    = 0;       /* number of cached terms       */
    static size_t  cap    = 0;       /* capacity of terms array      */
    static idx_t   last_k = 0;       /* largest k examined so far    */

    while (len < n)                    /* find enough terms          */
    {
        ++last_k;
        idx_t k = last_k;

        /* Modulus k*(k+1) – compute in 128‑bit to avoid overflow   */
        unsigned __int128 mod128 = (unsigned __int128)k * (k + 1);

        /* F(k) modulo k*(k+1) */
        unsigned __int128 fk128 = fib_mod_u128(k, mod128);

        if (fk128 == 0)                /* divisible condition        */
        {
            /* Ensure capacity of the terms array */
            if (len == cap)
            {
                cap = cap ? cap << 1 : 64;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[len++] = (seq_t)k;   /* store the k value           */
        }
    }
    return terms[n - 1];
}
