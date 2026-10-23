/*
 * A145281  a(n) is the least prime such that (ceiling(sqrt(a(n)*p_n)))^2 -
 *          a(n)*p_n is a perfect square, where p_n is the n-th prime.
 * https://oeis.org/A145281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   get_prime, is_prime, isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A145281
 *
 *   a(n) = the least prime q such that ceil(sqrt(q * p_n))^2 - q * p_n
 *          is a perfect square,
 *   where p_n is the n-th prime (p_1 = 2).
 *
 * The implementation stores all primes up to the largest p_n requested
 * in a static array, extending it on demand with a simple primality
 * test (is_prime from the utils).  For each candidate prime q <= p_n,
 * it evaluates the condition using integer arithmetic on seq_t;
 * multiplication and addition are performed with the usual operators,
 * as they are defined for both built‑in integer types and the arbitrary–
 * precision class used by the build.  All temporary values stay
 * bounded within the 64‑bit range for the problem limits (n <= 10000),
 * so the algorithm is well below the 2‑second per call requirement.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"
#include "../utils/isqrt_seq.h"

/*--------------------------------------------------------------------
 * Prime cache
 *--------------------------------------------------------------------*/
static idx_t *prime_list   = NULL;   /* dynamic array of primes      */
static size_t prime_count  = 0;      /* number of primes stored      */
static size_t prime_cap    = 0;      /* current allocated capacity   */
static idx_t  primes_max   = 0;      /* maximum prime for which list
                                      * is known to be complete       */

/* Ensure that prime_list contains all primes <= limit. */
static void
ensure_primes_up_to(idx_t limit)
{
    if (limit <= primes_max)
        return;                     /* already up to date          */

    /* initialise list if empty */
    if (prime_count == 0) {
        prime_cap = 16;
        prime_list = (idx_t *)malloc(prime_cap * sizeof(idx_t));
        if (!prime_list)            /* allocation failure          */
            return;
        prime_list[0] = 2;
        prime_count = 1;
    }

    idx_t last = prime_list[prime_count - 1];

    for (idx_t cand = last + 1; cand <= limit; ++cand) {
        if (is_prime(cand)) {
            if (prime_count == prime_cap) {
                prime_cap <<= 1;
                prime_list = (idx_t *)realloc(prime_list,
                                              prime_cap * sizeof(idx_t));
                if (!prime_list)        /* allocation failure      */
                    return;
            }
            prime_list[prime_count++] = cand;
        }
    }

    primes_max = limit;             /* mark cache up to this limit */
}

/*--------------------------------------------------------------------
 * Sequence function
 *--------------------------------------------------------------------*/
seq_t
a(idx_t n)
{
    /* p_n : n-th prime (1‑based) */
    idx_t p_n = (idx_t)get_prime(n);

    /* make sure we can look up all primes up to p_n */
    ensure_primes_up_to(p_n);

    /* search least prime q <= p_n satisfying the condition */
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t q = prime_list[i];
        if (q > p_n)
            break;                  /* theorem: a(n) ≤ p_n               */

        seq_t prod = (seq_t)q * (seq_t)p_n;       /* q * p_n                 */
        seq_t k   = isqrt_seq(prod);              /* floor(sqrt(prod))      */
        seq_t k2  = k * k;                         /* k^2                   */
        if (k2 != prod)                          /* not a perfect square   */
            k++;                                  /* ceil(sqrt(prod))       */
        k2 = k * k;                               /* now exact square      */
        seq_t d  = k2 - prod;                     /* difference             */
        seq_t sq = isqrt_seq(d);                  /* floor(sqrt(d))        */
        if (sq * sq == d)                         /* d is a perfect square */
            return (seq_t)q;                      /* smallest such q         */
    }

    /* Under the theorem a solution always exists; this path is never
     * reached for correct inputs, but it keeps the function robust. */
    return (seq_t)p_n;
}
