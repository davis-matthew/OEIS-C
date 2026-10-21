/*
 * A069210  a(1)=a(2)=1, a(n+2) = a(n+1)+1 if sign(sin(a(n+1))) =
 *          sign(sin(a(n))), a(n+2) = a(n)+1 otherwise.
 * https://oeis.org/A069210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1, a(2) = 1 a(n+2) = a(n+1)+1 if sign(sin(a(n+1))) =
 *            sign(sin(a(n))), a(n)+1 otherwise. sign(sin(x)) > 0 iff
 *            floor(x/π) is even
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   sign_positive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/sign_positive.h"

/*  Cache for computed terms (1‑based).  */
static seq_t *a_cache      = NULL;   /* a_cache[0] unused            */
static idx_t   a_cache_cap = 0;     /* number of allocated slots    */
static idx_t   a_cache_len = 0;     /* number of terms stored       */

/*  Ensure the cache holds indices up to 'needed'.  */
static void a_cache_ensure(idx_t needed)
{
    if (needed <= a_cache_cap)
        return;
    idx_t new_cap = a_cache_cap ? a_cache_cap * 2 : 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(a_cache, (new_cap + 1) * sizeof(seq_t));
    a_cache = tmp;
    a_cache_cap = new_cap;
}

/*  Sequence generator.  */
seq_t a(idx_t n)
{
    if (n == 1 || n == 2)
        return (seq_t)1;

    a_cache_ensure(n);

    /*  Initialise first two terms if cache is empty.  */
    if (a_cache_len < 2)
    {
        a_cache[1] = (seq_t)1;
        a_cache[2] = (seq_t)1;
        a_cache_len = 2;
    }

    /*  Compute missing terms up to n.  */
    for (idx_t i = a_cache_len + 1; i <= n; ++i)
    {
        seq_t prev  = a_cache[i - 1];
        seq_t prev2 = a_cache[i - 2];

        bool sign1 = sign_positive((uint64_t)prev);
        bool sign2 = sign_positive((uint64_t)prev2);

        if (sign1 == sign2)          /* same sign → +1 to previous term */
            a_cache[i] = prev + (seq_t)1;
        else                          /* otherwise → +1 to term before previous */
            a_cache[i] = prev2 + (seq_t)1;
    }

    a_cache_len = n;
    return a_cache[n];
}
