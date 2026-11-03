/*
 * A038941  Primes p such that 61 is a square mod p.
 * https://oeis.org/A038941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 978 (2026-11-03)
 * Helpers:   is_prime_seq, mem, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by generating primes in ascending order and selecting those
 * for which 61 is a quadratic residue modulo the prime, i.e. the Legendre
 * symbol (61/p) = 1. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
#include "../utils/powmod_u64.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined; sequence starts at n=1 */

    /* Static cache of primes satisfying the condition */
    static seq_t *list = NULL;     /* array of qualifying primes */
    static size_t list_len = 0;    /* number of stored primes */
    static size_t list_cap = 0;    /* current capacity of list */
    static uint64_t cand = 3;      /* next odd candidate to test */

    /* Expand cache until it contains at least n elements */
    while (list_len < (size_t)n)
    {
        /* Test primality of the current candidate */
        if (is_prime_seq((seq_t)cand))
        {
            /* Include the prime if 61 is a quadratic residue modulo cand */
            if (cand == 61U ||
                powmod_u64(61U, (cand - 1U) >> 1U, cand) == 1U)
            {
                /* Grow the array if necessary */
                if (list_len == list_cap)
                {
                    size_t new_cap = (list_cap == 0) ? 16 : list_cap << 1;
                    seq_t *tmp = mem_resize(seq_t, list, new_cap);
                    if (!tmp) abort();   /* allocation failure */
                    list = tmp;
                    list_cap = new_cap;
                }
                list[list_len++] = (seq_t)cand;
            }
        }
        cand += 2;  /* only odd numbers need to be tested */
    }

    return list[(size_t)n - 1];
}
