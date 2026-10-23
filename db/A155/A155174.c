/*
 * A155174  Long leg B of primitive Pythagorean triangles such that perimeters
 *          are Averages of twin prime pairs, q=p+1, a=q^2-p^2, c=q^2+p^2,
 *          b=2*p*q, s=a+b+c, s-+1 are primes.
 * https://oeis.org/A155174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 16, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A155174 – Long leg b = 2*p*(p+1) of primitive Pythagorean triples
 *  with p such that the perimeter s = 2*(p+1)*(2p+1) satisfies that
 *  both s‑1 and s+1 are prime.
 *
 *  This implementation builds the sequence on demand, caching the
 *  already‑computed terms.  It works for any seq_t implementation
 *  (32/64/128‑bit unsigned or arbitrary‑precision) and for any
 *  idx_t (uint64_t).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache      = NULL;   /* cached sequence terms      */
static size_t cache_len  = 0;      /* number of terms cached     */
static size_t cache_cap  = 0;      /* allocated capacity of cache*/
static idx_t  last_p     = 0;      /* last p examined             */

/* Ensure cache has room for at least 'need' elements.  C++ requires an explicit cast
 * from void* returned by realloc to the appropriate pointer type. */
static void grow_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                  /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth element of A155174 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1 – unused slot */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n)
    {
        for (idx_t p = last_p + 1; ; ++p)
        {
            seq_t pp = (seq_t)p;               /* cast once for speed */
            seq_t s  = (seq_t)2 * (pp + 1) * (pp * 2 + 1);   /* s = 2*(p+1)*(2p+1) */

            if (is_prime_big(s - 1) && is_prime_big(s + 1))
            {
                seq_t b = (seq_t)2 * pp * (pp + 1);  /* b = 2*p*(p+1) */
                grow_cache(cache_len + 1);
                cache[cache_len++] = b;
                last_p = p;
                break;                          /* next term found */
            }
        }
    }

    return cache[(size_t)n - 1];
}
