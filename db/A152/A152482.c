/*
 * A152482  Even numbers which are not the sum of 2 even semiprimes.
 * https://oeis.org/A152482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A152482: Evening number not the sum of two even semiprimes.
   The sequence is computed by testing each integer m for representability
   as a sum of two primes (since a sum of two even semiprimes equals 2*(p+q)).
   Each non‑representable m contributes a term 2*m to the sequence. */

/* Include required headers */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static bool initialized = false;
static idx_t limit = 2000000;            /* initial sieve bound          */
static bool *is_prime = NULL;            /* prime test array             */
static uint64_t *prime_list = NULL;      /* list of primes up to limit   */
static size_t prime_count = 0;           /* number of primes             */
static seq_t *terms = NULL;              /* stored sequence terms        */
static size_t terms_len = 0;             /* number of terms stored       */
static size_t terms_cap = 0;             /* capacity of terms array      */
static idx_t current_m = 1;              /* next integer to test         */

/* Build the sieve of Eratosthenes up to 'limit' and populate prime_list. */
static void build_sieve(idx_t lim)
{
    free(is_prime);
    free(prime_list);

    is_prime = (bool *)calloc(lim + 1, sizeof(bool));
    if (!is_prime) exit(1);           /* allocation failure */

    is_prime[0] = is_prime[1] = true;   /* mark 0 and 1 as composite */

    for (idx_t i = 2; (idx_t)(i * i) <= lim; ++i)
        if (!is_prime[i])
            for (idx_t j = i * i; j <= lim; j += i)
                is_prime[j] = true;

    /* count primes */
    prime_count = 0;
    for (idx_t i = 2; i <= lim; ++i)
        if (!is_prime[i]) ++prime_count;

    prime_list = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    if (!prime_list) exit(1);

    size_t idx = 0;
    for (idx_t i = 2; i <= lim; ++i)
        if (!is_prime[i]) prime_list[idx++] = i;
}

/* Check whether integer m can be written as the sum of two primes. */
static bool is_representable(idx_t m)
{
    if (m < 2) return false;

    if (m & 1) {                     /* m is odd */
        if (m == 3) return false;    /* 3 = 2+1 , 1 not prime */
        return !is_prime[m - 2];      /* m-2 must be prime */
    }

    /* m is even */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > m / 2) break;
        if (!is_prime[m - p]) return true;
    }
    return false;
}

/* Ensure that there are at least 'needed' terms stored in 'terms'. */
static void ensure_terms(idx_t needed)
{
    while (terms_len < needed) {
        /* If current_m exceeds current sieve limit, enlarge the sieve. */
        if (current_m > limit) {
            limit *= 2;
            build_sieve(limit);
        }

        if (!is_representable(current_m)) {
            /* Add 2*current_m to the sequence. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap == 0 ? 256 : terms_cap * 2;
                seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_terms) exit(1);
                terms = new_terms;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)(current_m * 2);
        }
        ++current_m;
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        build_sieve(limit);
        terms_cap = 0;
        terms_len = 0;
        terms = NULL;
        current_m = 1;
        initialized = true;
    }

    ensure_terms(n);
    return terms[n - 1];
}
