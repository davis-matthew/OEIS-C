/*
 * A162857  Primes of the form 4p - 1, p a prime.
 * https://oeis.org/A162857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162857:  primes of the form 4p-1, where p itself is prime.
   Each term is 4p-1 for the n-th prime p such that 4p-1 is also prime. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/next_prime.h"     /* uint64_t next_prime(uint64_t start); */

seq_t a(idx_t n)
{
    if (n == 0)                     /* Undefined for n = 0; return 0 as a safe default */
        return 0;

    idx_t  count = 0;               /* How many suitable primes found so far      */
    uint64_t p = 2;                 /* The next candidate prime (p itself)          */
    seq_t   result = 0;              /* Value to be returned when count reaches n  */

    while (count < n) {
        /* Candidate value of the sequence = 4p - 1 */
        seq_t cand = (seq_t)4 * (seq_t)p - 1;

        if (is_prime_big(cand)) {   /* 4p-1 is prime?                       */
            count++;                /* Found the next term of the sequence   */
            if (count == n)
                result = cand;      /* Save the n‑th term                     */
        }

        /* Move to the next prime p.  next_prime returns the
           smallest prime strictly greater than its argument,
           so passing the current p gives the correct next prime. */
        p = next_prime(p);
    }

    return result;
}
