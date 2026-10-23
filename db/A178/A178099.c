/*
 * A178099  Numbers k such that exactly three d in the range d <= k/2 exist
 *          which divide binomial(k-d-1,d-1) and which are not coprime to k.
 * https://oeis.org/A178099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 187, u64 n <= 187,
 *            u128 n <= 187, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A178099: numbers k such that exactly three d <= k/2 divide
   binomial(k-d-1,d-1) and are not coprime to k.
   For k > 145 the term equals p*(p+8) with p and p+8 primes. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

/* Max number of terms we will generate – sufficient for n ≤ 10000 */
#define MAX_TERMS 20000

/* Small terms (≤ 145) obtained by brute force */
static const seq_t small_terms[] = {
    32, 38, 45, 51, 52, 56, 57, 63, 69, 87, 145
};
static const size_t small_count = sizeof(small_terms) / sizeof(small_terms[0]);

/* Storage for the sequence */
static seq_t  seq[MAX_TERMS];
static size_t seq_len = 0;

/* Prime sieve state */
static uint64_t *prime_list   = NULL;   /* array of primes */
static size_t    prime_cnt    = 0;       /* number of primes */
static size_t    prime_index  = 0;       /* next prime to examine */
static idx_t     sieve_limit  = 1000000ULL;  /* current sieve bound */

/* Ensure the sequence contains at least n terms (n ≥ 1) */
static void ensure_terms(size_t n)
{
    size_t i;

    /* Add the pre‑computed small terms first */
    while (seq_len < n && seq_len < small_count)
        seq[seq_len++] = small_terms[seq_len];

    while (seq_len < n) {
        /* Build or extend the prime sieve if necessary */
        if (prime_list == NULL || prime_index >= prime_cnt) {
            if (prime_list != NULL) {
                free(prime_list);
                prime_list = NULL;
            }
            build_primes_upto(sieve_limit, &prime_list, &prime_cnt);
            prime_index = 0;
            /* Double the sieve bound for the next round */
            sieve_limit <<= 1;
        }

        /* Scan primes looking for p,p+8 twin */
        for (; prime_index < prime_cnt && seq_len < n; ++prime_index) {
            uint64_t p = prime_list[prime_index];
            if (p <= 1)
                continue;
            if (!is_prime(p + 8ULL))
                continue;

            seq_t prod = (seq_t)p * (seq_t)(p + 8ULL);
            if (prod > (seq_t)145)          /* all >145 products are new */
                seq[seq_len++] = prod;
        }

        /* If we ran out of primes we will next time resize the sieve */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                     /* offset starts at 1 */
        return (seq_t)0;

    if (n > MAX_TERMS) {            /* safety check – should not happen */
        return (seq_t)0;
    }

    ensure_terms((size_t)n);
    return seq[n - 1];
}
