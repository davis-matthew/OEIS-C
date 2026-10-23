/*
 * A156124  Primes p, such that p+72 and p+144 are both prime.
 * https://oeis.org/A156124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that p+72 and p+144 are also prime
 * Range:     u8 n <= 10, u16 n <= 391, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *list      = NULL;  /* cache of found terms              */
static size_t list_len  = 0;      /* number of terms stored            */
static size_t list_cap  = 0;      /* allocated capacity of list        */
static idx_t next_candidate = 3;  /* next odd integer to test (>=3)    */

seq_t a(idx_t n)
{
    /* a(1) is the first term; ignore calls with n == 0. */
    if (n == 0) return (seq_t)0;

    /* Grow the list until we have at least n elements. */
    while (list_len < (size_t)n)
    {
        idx_t p = next_candidate;
        next_candidate += 2;      /* advance to next odd integer */

        /* test the condition p, p+72, p+144 primes */
        if (is_prime(p) &&
            is_prime(p + 72) &&
            is_prime(p + 144))
        {
            /* ensure space for a new element */
            if (list_len == list_cap)
            {
                list_cap = list_cap ? list_cap * 2 : 16;
                list = (seq_t *)realloc(list, list_cap * sizeof(seq_t));
            }
            list[list_len++] = (seq_t)p;
        }
    }

    return list[n - 1];
}
