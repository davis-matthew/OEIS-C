/*
 * A064193  Numbers whose product of divisors and sum of unitary divisors are
 *          both squares.
 * https://oeis.org/A064193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 166, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, isqrt_uint64, pow_seq, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose product of divisors and sum of unitary divisors
   are both squares (OEIS A064193).  The function generates terms by
   iterating over integers, factorising each one and checking the two
   conditions below:

   * For each prime exponent a_i in n = ∏p_i^{a_i} let d = ∏(a_i+1)
     (number of divisors).  The product of divisors is n^{d/2}.  Its
     exponents are a_i*d/2, so a_i*d must be a multiple of 4 for all i
     (this also forces a perfect square when n is a square, i.e. a_i
     must be a multiple of 4).

   * The sum of unitary divisors is S_u(n)=∏(1+p_i^{a_i}).  This must
     be a perfect square.  We compute the product exactly in arbitrary
     precision (seq_t) and test whether its square root is integral.

   The algorithm runs in O(n log n) time for n up to about 10^4,
   comfortably under the 2‑second limit.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sieve_primes.h"
#include "../utils/floor_sqrt.h"
#include "../utils/pow_seq.h"
#include "../utils/isqrt_uint64.h"

static idx_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t sieve_limit = 0;

/* Ensure that prime_list contains all primes ≤ limit. */
static void
ensure_primes(idx_t limit)
{
    if (sieve_limit >= limit)
        return;
    if (prime_list != NULL)
        free(prime_list);
    sieve_primes(limit, &prime_list, &prime_count);
    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    /* Linear search for the nth element. */
    idx_t found = 0;
    idx_t candidate = 0;          /* 0 → 1 on first loop iteration */

    /* Temporary storage for factorisation: enough for most numbers. */
    const size_t MAX_FACT = 32;
    idx_t primes[MAX_FACT];
    int   exps  [MAX_FACT];

    while (found < n) {
        ++candidate;
        idx_t x = candidate;

        /* Prepare for factorisation. */
        size_t pcnt = 0;
        idx_t tmp = x;
        idx_t maxp = isqrt_uint64(tmp);
        ensure_primes(maxp);

        for (size_t i = 0; i < prime_count && prime_list[i] <= maxp; ++i) {
            idx_t p = prime_list[i];
            if ((uint64_t)p * (uint64_t)p > (uint64_t)tmp)
                break;
            if (tmp % p == 0) {
                int cnt = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++cnt;
                }
                primes[pcnt] = p;
                exps[pcnt] = cnt;
                ++pcnt;
                maxp = isqrt_uint64(tmp);
            }
        }
        if (tmp > 1) {
            primes[pcnt] = tmp;
            exps[pcnt] = 1;
            ++pcnt;
        }

        /* Compute divisor count d = ∏(a_i+1). */
        idx_t d = 1;
        for (size_t i = 0; i < pcnt; ++i) {
            d *= (idx_t)(exps[i] + 1);
        }

        /* Check product‑of‑divisors square condition:
           a_i*d must be a multiple of 4 for each i. */
        bool prod_ok = true;
        for (size_t i = 0; i < pcnt; ++i) {
            if (((idx_t)exps[i] * d) % 4 != 0) {
                prod_ok = false;
                break;
            }
        }
        if (!prod_ok)
            continue;   /* candidate rejected */

        /* Compute sum of unitary divisors S_u = ∏(1 + p_i^{a_i}) */
        seq_t su = 1;
        for (size_t i = 0; i < pcnt; ++i) {
            seq_t p_pow = pow_seq((seq_t)primes[i], exps[i]);   /* p_i^{a_i} */
            seq_t term  = 1 + p_pow;                           /* 1 + p_i^{a_i} */
            su *= term;
        }

        /* Check if S_u is a perfect square. */
        seq_t sq = floor_sqrt(su);
        if (sq * sq != su)
            continue;   /* candidate rejected */

        /* Candidate satisfies both conditions. */
        ++found;
    }

    return (seq_t)candidate;
}
