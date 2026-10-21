/*
 * A055398  Result of fourth stage of sieve of Eratosthenes (after eliminating
 *          multiples of 2, 3, 5, 7).
 * https://oeis.org/A055398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A055398: the fourth stage of the sieve of Eratosthenes.
   a(n) is the nth integer ≥ 2 that is either 2, 3, 5, 7 or has no prime
   divisor among {2, 3, 5, 7}. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *a_cache = NULL;          /* cached terms                          */
static size_t a_cache_len = 0;          /* number of cached terms                */
static size_t a_cache_cap = 0;          /* allocated capacity of a_cache         */
static seq_t a_next = 2;                /* next integer to test                  */

static void a_ensure(idx_t n)
{
    while (a_cache_len < (size_t)n)
    {
        seq_t val = a_next;
        bool ok = false;

        if (val == 2 || val == 3 || val == 5 || val == 7)
            ok = true;
        else if ((val % 2 != 0) && (val % 3 != 0) && (val % 5 != 0) && (val % 7 != 0))
            ok = true;

        if (ok)
        {
            if (a_cache_len >= a_cache_cap)
            {
                size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                a_cache = tmp;
                a_cache_cap = new_cap;
            }
            a_cache[a_cache_len++] = val;
        }
        a_next++;   /* move to next integer */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, return 0 as a fallback */
    a_ensure(n);
    return a_cache[n - 1];
}
