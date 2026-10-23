/*
 * A118071  Primes which are the sum of a twin prime pair + 1.
 * https://oeis.org/A118071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 110, u32 n <= 42, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A118071
 * a(n) = 2*p + 3, where p and p+2 are consecutive twin primes.
 * The sequence starts: 13,37,61,277,...
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ensure_capacity.h"
#include "../utils/is_prime_big.h"

static uint64_t *prime_list = NULL;   /* primes up to current limit          */
static size_t prime_cnt    = 0;       /* number of primes in prime_list      */
static uint64_t limit      = 0;       /* current sieve limit                 */
static size_t twin_start   = 0;       /* index of lower twin prime to test   */

static seq_t *terms       = NULL;    /* cached sequence values              */
static size_t terms_cnt   = 0;
static size_t terms_cap   = 0;

/* extend the prime sieve so that it contains all primes ≤ new_limit */
static void extend_primes(uint64_t new_limit)
{
    uint64_t *new_list = NULL;
    size_t new_cnt = 0;
    build_primes_upto(new_limit, &new_list, &new_cnt);
    free(prime_list);
    prime_list = new_list;
    prime_cnt  = new_cnt;
    limit      = new_limit;
}

/* make sure that the sequence contains at least n terms   */
static void ensure_terms(idx_t n)
{
    if (n == 0) return;

    /* Initial sieve */
    if (limit == 0)
        extend_primes(200000ULL);

    while (terms_cnt < (size_t)n)
    {
        /* Make sure we can look at the next candidate twin prime pair */
        while (twin_start + 1 >= prime_cnt)
            extend_primes(limit * 2ULL);

        /* Check if prime_list[twin_start] and prime_list[twin_start+1]
         * form a twin prime pair */
        if (prime_list[twin_start + 1] == prime_list[twin_start] + 2ULL)
        {
            /* Candidate = lower twin prime * 2 + 3 */
            seq_t candidate = (seq_t)prime_list[twin_start];
            candidate = candidate * (seq_t)2 + (seq_t)3;
            if (is_prime_big(candidate))
            {
                ensure_capacity(terms_cnt + 1, &terms_cap,
                               (void **)&terms, sizeof(seq_t));
                terms[terms_cnt++] = candidate;
                /* After adding, we may have enough terms -> exit if desired */
                /* but the outer loop will check again */
            }
        }
        /* Move to next prime for the next trial */
        twin_start++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; indices <1 are undefined */
        return (seq_t)0;
    ensure_terms(n);
    return terms[n - 1];
}
