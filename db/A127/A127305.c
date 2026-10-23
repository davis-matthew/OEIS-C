/*
 * A127305  Primes p such that p + (sum of the prime factors of p-1) + (sum of
 *          prime factors of p+1) is prime.
 * https://oeis.org/A127305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p such that p + (sum of prime factors of p-1) + (sum of
 *            prime factors of p+1) is a prime. a(1)=2, a(2)=13, a(3)=47, …
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 531, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises found terms and builds them on demand
   using a simple prime sieve and trial division.  It works for n up
   to about 10 000 comfortably in the 2‑second limit. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_big.h"

/* ----------  Memoised state  ---------- */
static seq_t *seq_vals   = NULL;   /* cached terms                  */
static size_t seq_cnt    = 0;       /* number of terms stored so far */
static size_t seq_cap    = 0;       /* current array capacity        */

/* Prime list used for factorisation of numbers up to a limit. */
static uint64_t *prime_list = NULL;   /* pre‑computed primes            */
static size_t prime_cnt   = 0;         /* count of primes in prime_list  */
static uint64_t prime_lim = 0;         /* largest n for which primes are pre‑computed */

/* Next candidate prime to test.  Uses uint64_t for simplicity. */
static uint64_t next_candidate = 2;

/* ----------  Helper: expand prime list to cover limit  ---------- */
static void ensure_prime_list(uint64_t limit)
{
    if (limit <= prime_lim) return;

    uint64_t *new_list = NULL;
    size_t new_cnt    = 0;
    build_primes_upto(limit, &new_list, &new_cnt);

    if (prime_list) free(prime_list);
    prime_list = new_list;
    prime_cnt  = new_cnt;
    prime_lim  = limit;
}

/* ----------  Helper: sum of prime factors with multiplicity  ---------- */
static seq_t sum_prime_factors(uint64_t n)
{
    if (n <= 1) return 0;

    /* Need primes up to sqrt(n) */
    uint64_t sf = (uint64_t)floor_sqrt((seq_t)n);
    ensure_prime_list(sf);

    seq_t sum = 0;
    uint64_t tmp = n;
    size_t i = 0;

    while (tmp > 1 && i < prime_cnt) {
        uint64_t p = prime_list[i];
        if (p * p > tmp) break;                /* remaining factor is prime   */
        while (tmp % p == 0) {
            sum += (seq_t)p;
            tmp /= p;
        }
        ++i;
    }
    if (tmp > 1)                /* last remaining prime factor */
        sum += (seq_t)tmp;

    return sum;
}

/* ----------  Sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; undefined for 0 */
        return 0;

    /* Grow cache until it contains at least n terms. */
    while (seq_cnt < (size_t)n) {
        /* --- search for the next qualifying prime --- */
        for (;;) {
            /* Skip even numbers after 2. */
            if (next_candidate > 2 && (next_candidate & 1) == 0)
                next_candidate++;

            /* Test primality of the candidate. */
            if (is_prime_big((seq_t)next_candidate)) {
                uint64_t p  = next_candidate;
                uint64_t m1 = p - 1;   /* p‑1  */
                uint64_t m2 = p + 1;   /* p+1  */

                seq_t sum1 = sum_prime_factors(m1);
                seq_t sum2 = sum_prime_factors(m2);

                seq_t total = (seq_t)p + sum1 + sum2;
                if (is_prime_big(total)) {
                    /* Store the prime in the cache. */
                    if (seq_cnt == seq_cap) {
                        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
                        seq_t *tmp = (seq_t*)realloc(seq_vals, new_cap * sizeof(seq_t));
                        if (!tmp) abort();
                        seq_vals = tmp;
                        seq_cap  = new_cap;
                    }
                    seq_vals[seq_cnt++] = (seq_t)p;
                    /* Advance to the next candidate for future searches. */
                    next_candidate += 2;
                    break;            /* term added; exit inner loop */
                }
            }

            /* Advance to the next odd number. */
            next_candidate += 2;
        }
    }

    return seq_vals[n - 1];
}
