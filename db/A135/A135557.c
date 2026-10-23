/*
 * A135557  Consider the primes which are congruent to 2 or 3 modulo 5. List
 *          the sum of any consecutive pair if that sum is congruent to 0
 *          modulo 10.
 * https://oeis.org/A135557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n‑th sum of two consecutive primes that are ≡ 2 or 3
 *            (mod 5) and whose sum is ≡ 0 (mod 10). The primes are considered
 *            in their natural order and the pairs are taken from the
 *            subsequence of such primes
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Global mutable cache for the sequence.  The data are retained
   between calls so that multiple invocations with arbitrary
   n are fast. */
static uint64_t *primes      = NULL;     /* list of primes found so far    */
static size_t   primes_cnt   = 0;        /* number of primes stored        */
static size_t   primes_cap   = 0;        /* current capacity of primes array*/
static uint64_t next_prime   = 2;        /* next integer to test for primality */

static seq_t *sums          = NULL;     /* array of sequence values       */
static size_t sums_cnt      = 0;        /* number of sums computed        */
static size_t sums_cap      = 0;        /* capacity of sums array         */

static idx_t    processed_idx = 0;      /* next prime index to process for sums */
static uint64_t last_allowed   = 0;      /* last prime that satisfied 2/3 mod 5 */
static bool     has_last      = false;   /* whether last_allowed is set     */

/* Utility: obtain the next prime, extending the primes array if
   necessary.  The algorithm is straightforward trial division
   using the primes that have already been found. */
static void generate_next_prime(void)
{
    for (;;)
    {
        uint64_t p = next_prime++;
        bool is_prime = true;

        for (size_t i = 0; i < primes_cnt; ++i)
        {
            uint64_t pr = primes[i];
            __uint128_t sq = (__uint128_t)pr * pr;
            if (sq > p) break;
            if (p % pr == 0) { is_prime = false; break; }
        }

        if (is_prime)
        {
            if (primes_cnt >= primes_cap)
            {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = p;
            return;
        }
    }
}

/* Compute the next sum in the sequence, updating the sums array
   and the processing indices.  The function is idempotent with
   respect to already computed values. */
static void extend_sum_array_for(idx_t need)
{
    while (sums_cnt < (size_t)need)
    {
        /* Generate more primes if we have exhausted the known ones. */
        while (processed_idx >= primes_cnt)
            generate_next_prime();

        uint64_t p = primes[processed_idx++];

        /* Check if this prime is ≡ 2 or 3 (mod 5). */
        if (p % 5 == 2 || p % 5 == 3)
        {
            if (has_last)
            {
                uint64_t sum = last_allowed + p;
                if ((sum % 10) == 0)
                {
                    if (sums_cnt >= sums_cap)
                    {
                        size_t new_cap = sums_cap ? sums_cap * 2 : 64;
                        seq_t *tmp = (seq_t *)realloc(sums, new_cap * sizeof(seq_t));
                        if (!tmp) abort();
                        sums = tmp;
                        sums_cap = new_cap;
                    }
                    sums[sums_cnt++] = (seq_t)sum;
                }
            }
            last_allowed = p;
            has_last = true;
        }
        /* Non‑allowed primes do not break the “consecutive” chain; the
           last_allowed value remains unchanged until another allowed
           prime is found. */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)         /* offset is 1; undefined below 1 */
        return (seq_t)0;

    extend_sum_array_for(n);
    return sums[(size_t)n - 1];
}
