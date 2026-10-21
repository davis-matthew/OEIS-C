/*
 * A096525  Primes of form n.0.n+1, where '.' represents concatenation. Or,
 *          primes of form 10^(k+1)*n + n + 1, where k is the number of digits
 *          in n.
 * https://oeis.org/A096525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 571, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_of_idx, isPrimeGeneric, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A096525:  primes of the form n·10^(k+1)+n+1, where k is the number of
    decimal digits of n.  a(1)=607, a(2)=809, a(3)=20021, …    */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>   /* for uintptr_t used by realloc */

/*  The following utility functions are provided by the test harness.
 *  In the code below we simply include their headers.  The names of
 *  the headers match the function names with a .h suffix.                       */
#include "../utils/pow10.h"          /*  seq_t pow10(idx_t e);          */
#include "../utils/isPrimeGeneric.h" /*  bool isPrimeGeneric(seq_t n);   */
#include "../utils/digits_of_idx.h"

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t   size   = 0;   /* number of stored primes          */
    static size_t   capacity = 0; /* allocated space for cache        */
    static idx_t   next_n = 1;    /* next n to test for primality    */

    /* generate primes until we have at least n of them */
    while (size < n) {
        uint64_t k = digits_of_idx(next_n);     /* number of digits of next_n */
        seq_t  pow = pow10((idx_t)(k + 1));     /* 10^(k+1) */
        seq_t  val = (seq_t)next_n * pow + (seq_t)next_n + 1; /* value to test */

        if (isPrimeGeneric(val)) {             /* test primality */
            if (size == capacity) {            /* grow cache if needed */
                size_t new_cap = capacity ? capacity * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* allocation failed */
                cache = tmp;
                capacity = new_cap;
            }
            cache[size++] = val;                /* store prime */
        }
        ++next_n;                               /* try next n */
    }

    return cache[n - 1];
}
