/*
 * A030522  Product of first n palindromic primes minus 1.
 * https://oeis.org/A030522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (∏_{k=1}^{n} p_k) – 1, where p_k denotes the k‑th
 *            palindromic prime
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   is_palindrome, is_prime_big, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_palindrome.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;     /* products minus one: a(1), a(2), …   */
static size_t  cap  = 0;        /* allocated capacity                   */
static size_t  sz   = 0;        /* number of cached terms                */
static idx_t   last = 1;        /* last integer tested for primality     */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1 – guard                  */

    /* Expand cache to hold values up to n                                */
    if ((size_t)n > sz)
    {
        if ((size_t)n > cap)                 /* grow capacity                    */
        {
            size_t newcap = cap ? cap * 2 : 8;
            if (newcap < (size_t)n) newcap = (size_t)n;
            cache = seq_realloc(cache, cap, newcap);
            cap = newcap;
        }

        /* Generate palindromic primes until we have n terms              */
        while (sz < (size_t)n)
        {
            seq_t cand;
            /* find next palindromic prime                                  */
            do
            {
                ++last;
                cand = (seq_t)last;
            }
            while (!(is_palindrome(cand, 10) && is_prime_big(cand)));

            seq_t prod;
            if (sz == 0)
                prod = cand;                /* first product of one prime      */
            else
            {
                seq_t prev_prod = cache[sz - 1] + (seq_t)1; /* reverse -1   */
                prod = seq_mul(prev_prod, cand);
            }

            cache[sz] = prod - (seq_t)1;     /* store a(n)                       */
            ++sz;
        }
    }

    return cache[n - 1];                   /* offset 1                          */
}
