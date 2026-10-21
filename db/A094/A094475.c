/*
 * A094475  Primes of form 2^n + 5^n.
 * https://oeis.org/A094475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime of the form 2^k + 5^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   isPrime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine searches increasing exponents k, computes 2^k + 5^k,
   tests each candidate for primality, and stores the primes in order.
   Results are memoised so calls may arrive in any order.  The code
   accepts the supplied types seq_t and idx_t, and relies on the
   helper functions isPrime (for 64‑bit integers) and
   is_prime_big (for arbitrary‑precision integers). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrime.h"
#include "../utils/is_prime_big.h"

static inline bool is_prime_seq(seq_t value)
{
    /* Use the 64‑bit test when the value fits in idx_t, otherwise
       use the generic big‑int routine. */
    if (sizeof(seq_t) <= sizeof(idx_t))
    {
        return isPrime((idx_t)value);
    }
    else
    {
        return is_prime_big(value);
    }
}

/* Compute base^exp using repeated multiplication.
   Works for arbitrary seq_t types as long as multiplication and
   addition are defined. */
static seq_t pow_int_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* out of range */

    /* Static storage for previously found primes. */
    static seq_t *primes = NULL;
    static size_t prime_len = 0, prime_cap = 0;

    /* Current values of 2^k and 5^k. */
    static seq_t pow2 = (seq_t)1, pow5 = (seq_t)1;
    static idx_t next_k = 0;          /* exponent already processed */

    /* Initialise on first call. */
    if (primes == NULL)
    {
        prime_cap = 16;
        primes = (seq_t *)malloc(prime_cap * sizeof(seq_t));
        if (primes == NULL)
            return (seq_t)0;   /* allocation failed */
    }

    /* Generate primes until we have at least n of them. */
    while (prime_len < (size_t)n)
    {
        /* Candidate for exponent next_k: 2^next_k + 5^next_k. */
        seq_t candidate = pow2 + pow5;

        if (is_prime_seq(candidate))
        {
            if (prime_len == prime_cap)
            {
                prime_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(primes, prime_cap * sizeof(seq_t));
                if (tmp == NULL)
                    return (seq_t)0;   /* realloc failed */
                primes = tmp;
            }
            primes[prime_len++] = candidate;
        }

        /* Advance to the next exponent. */
        pow2 *= (seq_t)2;
        pow5 *= (seq_t)5;
        ++next_k;
    }

    return primes[n - 1];
}
