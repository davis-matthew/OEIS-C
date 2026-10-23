/*
 * A144523  Triangular numbers n*(n+1)/2 with n and n+1 composite, where
 *          number of prime factors in n > number of prime factors in n+1.
 * https://oeis.org/A144523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 where n and n+1 are composite and the number of
 *            prime factors (with multiplicity) of n is greater than that of
 *            n+1. Off‑by‑one: a(1) = 36, a(2) = 210, …
 * Range:     u8 n <= 2, u16 n <= 82, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/bigomega.h"

/* Maximum number of sequence elements that the static cache can hold.
   This should be large enough for all practical uses (e.g., n up to
   200 000).  If it is exceeded the function will silently stop
   producing new terms. */
#define CACHE_CAP   200000

/* Static cache for already computed triangular numbers. */
static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;   /* Number of terms currently stored. */
static idx_t  last_checked = 1; /* Last n examined for the condition. */

/* Return the n‑th term of sequence A144523.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined, but harmless */
        return (seq_t)0;

    /* Grow the cache until it holds the requested term. */
    while (cache_len < n)
    {
        /* Examine the next integer n. */
        ++last_checked;
        idx_t cur = last_checked;

        /* Skip if either cur or cur+1 is prime. */
        if (is_prime(cur) || is_prime(cur + 1))
            continue;

        /* Compare the number of prime factors (with multiplicity). */
        seq_t omega_cur  = bigomega(cur);
        seq_t omega_curp = bigomega(cur + 1);
        if (!(omega_cur > omega_curp))
            continue;

        /* Compute the triangular number n*(n+1)/2 safely. */
        seq_t tri;
        if (cur & 1u)  /* cur is odd */
            tri = (seq_t)cur * (((seq_t)(cur + 1)) / 2u);
        else           /* cur is even */
            tri = (((seq_t)cur) / 2u) * (seq_t)(cur + 1);

        /* Store the result. */
        if (cache_len < CACHE_CAP)
            cache[cache_len++] = tri;
        else
            /* Cache is full – stop adding more terms. */
            break;
    }

    if (n <= cache_len)
        return cache[n - 1];
    else
        /* If the request is beyond the cache capacity, return 0.  In
           practice this should not happen for typical usage. */
        return (seq_t)0;
}
