/*
 * A089253  Numbers n such that 2n - 5 is a prime.
 * https://oeis.org/A089253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A089253.   Numbers n such that 2n-5 is prime.  
   a(n) = (p_n + 5)/2, where p_n is the n-th odd prime (p_1 = 3).  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Return the n-th term of the sequence.  
   For n >= 1.  The function is memoised so calls need not be in order. */
seq_t a(idx_t n)
{
    /* Array of odd primes already found (increasing order). */
    static uint64_t *primes = NULL;   /* dynamic array */
    static size_t   primes_cap = 0;   /* current capacity */
    static size_t   primes_len = 0;   /* number of primes stored */

    if (n == (idx_t)0) {
        /* Sequence is defined for n >= 1; return 0 for other inputs. */
        return (seq_t)0;
    }

    /* Generate odd primes until we have at least n of them. */
    if (primes_len < (size_t)n) {
        uint64_t candidate = (primes_len == 0) ? 3 : primes[primes_len - 1] + 2;

        while (primes_len < (size_t)n) {
            /* Trial division using the primes already known. */
            bool is_prime = true;
            for (size_t i = 0; i < primes_len; ++i) {
                uint64_t p = primes[i];
                if (p > candidate / p) break;   /* p * p > candidate */
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }

            if (is_prime) {
                /* Grow the array if needed. */
                if (primes_len == primes_cap) {
                    size_t new_cap = (primes_cap == 0) ? 16 : primes_cap * 2;
                    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                    if (!tmp) {
                        /* Allocation failed – in the test harness this should not happen. */
                        return (seq_t)0;
                    }
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_len++] = candidate;
            }

            candidate += 2;   /* skip even numbers */
        }
    }

    /* Retrieve the requested odd prime. */
    uint64_t p_n = primes[(size_t)n - 1];

    /* Compute (p_n + 5)/2 as a seq_t value. */
    seq_t result = (seq_t)p_n + (seq_t)5;
    result /= (seq_t)2;

    return result;
}
