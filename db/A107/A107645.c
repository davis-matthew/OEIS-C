/*
 * A107645  Numbers k such that the string 1311k is the decimal expansion of a
 *          prime number.
 * https://oeis.org/A107645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k such that 1311*10^digits(k)+k is prime
 * Range:     u8 n <= 19, u16 n <= 3600, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Provides the nth term of OEIS A107645 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Memoised results */
    static seq_t *arr = NULL;
    static size_t cap = 0;
    static size_t size = 0;
    /* State for the generation process */
    static uint64_t next_k = 0;          /* Next k to test             */
    static uint64_t pow10 = 10;          /* 10^digits(k)                */
    static uint64_t next_pow10 = 10;     /* Threshold when digits inc.  */

    /* Generate terms until we have at least n of them */
    while (size < n)
    {
        /* Update *pow10* when crossing a digit boundary */
        if (next_k >= next_pow10)
        {
            pow10 *= 10;
            next_pow10 *= 10;
        }

        /* Construct the candidate number 1311k */
        uint64_t candidate = 1311ULL * pow10 + next_k;

        /* Test primality */
        if (is_prime(candidate))
        {
            /* Ensure enough capacity */
            if (size >= cap)
            {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(arr, new_cap * sizeof(seq_t));
                if (!tmp) return 0;    /* Allocation failed – unlikely in tests */
                arr  = tmp;
                cap  = new_cap;
            }
            arr[size++] = (seq_t)next_k;
        }

        next_k++;
    }

    return arr[n - 1];
}
