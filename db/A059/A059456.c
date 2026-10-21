/*
 * A059456  Unsafe primes: primes not in A005385.
 * https://oeis.org/A059456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 4884, u32 n <= 6041, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Unsafe primes: primes p such that (p-1)/2 is not prime (sequence A059456) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static state across calls */
    static seq_t *unsafe = NULL;        /* array of found unsafe primes     */
    static size_t unsafe_size = 0;      /* current number of unsafe primes  */
    static size_t unsafe_cap  = 0;      /* capacity of the array            */
    static seq_t candidate = 2;         /* next integer to test             */

    if (n == 0)                /* offset is 1; ignore n==0          */
        return 0;

    /* Generate unsafe primes until we have at least n of them */
    while (unsafe_size < (size_t)n)
    {
        /* Test whether candidate is prime */
        bool isprime = is_prime_big(candidate);

        if (isprime)
        {
            /* (candidate-1)/2 must be checked for primality */
            seq_t mid  = (candidate - (seq_t)1) / (seq_t)2;
            bool safe  = is_prime_big(mid);

            if (!safe)                   /* unsafe prime found */
            {
                /* Grow array if necessary */
                if (unsafe_size + 1 > unsafe_cap)
                {
                    size_t new_cap = unsafe_cap ? unsafe_cap * 2 : (size_t)4;
                    if (new_cap < unsafe_size + 1)
                        new_cap = unsafe_size + 1;
                    seq_t *tmp = (seq_t *)realloc(unsafe,
                                                   new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();          /* memory allocation failed   */
                    unsafe = tmp;
                    unsafe_cap = new_cap;
                }
                unsafe[unsafe_size++] = candidate;
            }
        }

        /* Advance to the next integer to test */
        candidate++;
        if (candidate > (seq_t)2 && (candidate & (seq_t)1) == 0)
            candidate++;             /* skip even numbers > 2 */
    }

    return unsafe[(size_t)n - 1];
}
