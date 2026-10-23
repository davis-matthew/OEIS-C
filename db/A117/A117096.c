/*
 * A117096  Numbers n such that nextprime(2*n)<2*nextprime(n) (if p is prime
 *          then nextprime(p)=p).
 * https://oeis.org/A117096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A117096
 *  a(n) = the n‑th positive integer m such that
 *         nextprime(2*m) < 2*nextprime(m)
 *  (if p is prime then nextprime(p)=p).
 *
 *  Implementation:  a linear search with a helper that returns the
 *  smallest prime ≥ a given start.  The helper uses the supplied
 *  is_prime_big() routine from the utils package.  The sequence is
 *  memoised so that calls in any order remain fast.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static uint64_t _next_prime_uint64(uint64_t start)
{
    for (uint64_t p = start; ; ++p)
        if (is_prime_big((seq_t)p))
            return p;
}

seq_t a(idx_t n)
{
    /* memoised list of already found terms */
    static seq_t *terms = NULL;
    static size_t    len  = 0;   /* number of stored terms  */
    static size_t    cap  = 0;   /* current capacity         */
    static uint64_t  next = 1;   /* next integer to test    */

    /* grow the stored list as required */
    while ((size_t)n > len)
    {
        uint64_t c = next++;   /* candidate integer c      */

        uint64_t p = _next_prime_uint64(c);          /* nextprime(c)    */
        uint64_t q = _next_prime_uint64(2 * c);      /* nextprime(2c)   */

        if (q < 2 * p)                            /* property holds */
        {
            if (len == cap)                      /* grow storage   */
            {
                size_t newcap = (cap == 0) ? 8 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                if (!tmp) abort();                /* allocation fail */
                terms = tmp;
                cap   = newcap;
            }
            terms[len++] = (seq_t)c;              /* store term     */
        }
    }

    return terms[n - 1];   /* 1‑based indexing into the sequence */
}
