/*
 * A232882  Twin primes with digital root 8 or 1.
 * https://oeis.org/A232882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A232882: the n‑th member of the sequence of twin primes of the form (9 k‑1, 9 k + 1),
   which automatically have digital roots 8 and 1.  The implementation
   generates such twin primes on demand, storing them in a static array
   that persists across calls. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

/* Static cache of discovered twin primes.  Each prime is stored as
   a separate seq_t element; the array contains the primes in the
   order they appear in the sequence. */
static seq_t *list   = NULL;   /* dynamic array of seq_t values   */
static size_t len   = 0;       /* number of entries in list       */
static size_t cap   = 0;       /* allocated capacity of list      */
static uint64_t last_k = 1;    /* last k value tested (starting from 2) */

/* Ensure that 'list' has room for at least 'needs' elements.  The
   function uses a standard exponential growth strategy.  It casts
   the result of realloc to seq_t*, which is correct for both C and
   C++ (seq_t might be an arithmetic type or an arbitrary‑precision
   class). */
static void grow_list(size_t needs)
{
    if (needs <= cap)
        return;
    size_t newcap = cap == 0 ? 2 : cap << 1;
    while (newcap < needs)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(list, newcap * sizeof(seq_t));
    if (!tmp)
        abort();                   /* allocation failure is fatal */
    list = tmp;
    cap  = newcap;
}

/* Return the n‑th element of A232882.  The function is safe to call
   with n in any order; it remembers already‑computed values. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* not used in OEIS, but harmless */

    /* Generate twin primes until we have at least n elements in the
       cache.  Each twin pair contributes two elements to the sequence
       (the lower prime first, then the upper). */
    while (len < (size_t)n)
    {
        ++last_k;                     /* next candidate k (starting from 2) */
        uint64_t p1 = 9 * last_k - 1; /* lower twin prime candidate   */
        uint64_t p2 = 9 * last_k + 1; /* upper twin prime candidate   */

        if (is_prime_small(p1) && is_prime_small(p2))
        {
            /* Store both primes in the cache. */
            grow_list(len + 2);
            list[len++] = (seq_t)p1;
            list[len++] = (seq_t)p2;
        }
    }

    return list[n - 1];
}
