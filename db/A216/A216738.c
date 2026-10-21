/*
 * A216738  Primes p such that x^14 = -2 has no solution mod p.
 * https://oeis.org/A216738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes p such that x^14 = -2 has no solution mod p.
   Computation: for an odd prime p, let d = gcd(14, p-1).
   -2 is a 14th power iff (-2)^( (p-1)/d ) ≡ 1 (mod p).
   We generate primes incrementally and keep the primes
   that fail this test.  The list is memoised so that any order
   of calls returns the correct value.  The implementation
   uses trial‑division for primality and the provided modulus
   exponentiation routine to handle arbitrary–precision types.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

/* Euclidean gcd for uint64_t */
static uint64_t ugcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* test whether p (odd prime) has no solution of x^14 = -2 (mod p) */
static bool has_no_solution(uint64_t p)
{
    uint64_t m   = p - 1;
    uint64_t d   = ugcd(14ULL, m);
    uint64_t exp = m / d;
    seq_t res    = mod_pow((seq_t)(p - 2), exp, (seq_t)p);
    return res != (seq_t)1;
}

/*
 * The sequence a(n) is the nth prime in the list of primes satisfying
 * the above condition.  The implementation keeps a growing list of
 * such primes and generates more primes as required.
 */
seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *seq = NULL;    /* array of sequence elements */
    static size_t seq_cap = 0;   /* allocated capacity of seq */
    static idx_t seq_len = 0;    /* number of elements stored */

    static uint64_t *primes = NULL; /* list of all primes found so far */
    static size_t primes_cap = 0;
    static size_t primes_len = 0;

    static uint64_t cand = 2;      /* current candidate number */

    /* ensure we have at least n elements in seq */
    while (seq_len < n)
    {
        /* primality test via trial division using primes[0..primes_len-1] */
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i)
        {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime)
        {
            /* store the prime in primes list */
            if (primes_len == primes_cap)
            {
                size_t new_cap = (primes_cap == 0) ? 64 : primes_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = cand;

            /* evaluate the required condition for odd primes */
            if (cand > 2 && has_no_solution(cand))
            {
                if (seq_len == seq_cap)
                {
                    size_t new_cap = (seq_cap == 0) ? 64 : seq_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    seq = tmp;
                    seq_cap = new_cap;
                }
                seq[seq_len++] = (seq_t)cand;
            }
        }

        /* advance to next candidate */
        cand += (cand == 2 ? 1 : 2);
    }

    return seq[n - 1];
}
