/*
 * A200911  Primes of the form 5*n^3-3.
 * https://oeis.org/A200911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A200911: Primes of the form 5*n^3-3
    a(k) = the k-th prime of the form 5*m^3-3, k >= 1.
    Formula: 5*m^3 - 3  (m in N) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static idx_t total        = 0;        /* number of primes found so far */
    static idx_t next_k       = 1;        /* next m to test */
    static seq_t *primes      = NULL;     /* dynamic array of found primes */
    static size_t cap         = 0;        /* current capacity of array */

    if (n == 0)                /* offset behaviour not defined */
        return 0;

    /* generate primes until we have n of them */
    while (total < n)
    {
        seq_t m = (seq_t)next_k;   /* convert candidate to seq_t */
        seq_t val = 5 * m * m * m - 3;

        if (is_prime_seq(val))
        {
            /* grow array if necessary */
            if ((size_t)total >= cap)
            {
                size_t newcap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t*)realloc(primes, newcap * sizeof(seq_t));
                if (!tmp)   /* allocation failed – abort */
                    return 0;
                primes = tmp;
                cap = newcap;
            }
            primes[total++] = val;
        }
        ++next_k;
    }

    return primes[n - 1];
}
