/*
 * A237812  Primes p such that 2*p+1 and 2*p+13 are also prime.
 * https://oeis.org/A237812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 187, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 2*p+1 and 2*p+13 are also prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *table    = NULL;
    static size_t len      = 0;
    static size_t cap      = 0;
    static idx_t  last_chk = 1;        /* start checking from 2 */

    if (n == 0) return 0;              /* undefined, but safe */

    /* Ensure the table can hold at least n terms */
    while (len < (size_t)n)
    {
        /* Find the next prime p */
        while (true)
        {
            if (is_prime(last_chk)) break;
            ++last_chk;
        }
        /* Test 2*p+1 and 2*p+13 */
        idx_t p = last_chk;
        idx_t q1 = p * 2 + 1;
        idx_t q2 = p * 2 + 13;
        if (is_prime(q1) && is_prime(q2))
        {
            /* Grow the table if needed */
            if (len == cap)
            {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation error – unlikely */
                table = tmp;
                cap   = new_cap;
            }
            table[len++] = (seq_t)p;
        }
        /* Advance to the next integer */
        ++last_chk;
    }
    return table[n - 1];
}
