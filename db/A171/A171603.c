/*
 * A171603  Lesser of two consecutive primes such that half-sums are product
 *          of two distinct primes.
 * https://oeis.org/A171603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the lesser of two consecutive primes (p,q) such that
 *            (p+q)/2 is the product of two distinct primes
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below uses a lazy sieve and memoization to generate
   consecutive primes and test each pair.  It is valid C11 and C++17. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Prime cache.  The list grows as needed.  It is never shrunk.        */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list     = NULL;   /* prime numbers                */
static size_t    prime_count    = 0;      /* number of primes stored      */
static size_t    prime_capacity = 0;      /* allocated capacity           */

/* ------------------------------------------------------------------ */
/* Sequence cache.  Each entry holds the lesser prime of a valid pair.*/
/* ------------------------------------------------------------------ */
static seq_t *term_list     = NULL;      /* cached sequence terms        */
static size_t term_len      = 0;          /* number of terms stored       */
static size_t term_capacity = 0;          /* allocated capacity           */

/* ------------------------------------------------------------------ */
/* State used to generate prime pairs.                                 */
/* next_pair_index is the index of the first prime of the next pair.   */
/* ------------------------------------------------------------------ */
static size_t next_pair_index = 0;        /* index of left prime to test  */
static bool initialized = false;

/* ------------------------------------------------------------------ */
/* Ensure that the static data structures are created.                */
/* ------------------------------------------------------------------ */
static void ensure_initialized(void)
{
    if (initialized) return;

    prime_capacity = 16;
    prime_list = (uint64_t *)malloc(prime_capacity * sizeof(uint64_t));
    prime_list[prime_count++] = 2;
    prime_list[prime_count++] = 3;

    term_capacity = 16;
    term_list = (seq_t *)malloc(term_capacity * sizeof(seq_t));

    next_pair_index = 0;
    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Generate the next prime following the current last prime.          */
/* ------------------------------------------------------------------ */
static void generate_next_prime(void)
{
    uint64_t candidate = prime_list[prime_count - 1] + 2; /* skip evens */

    while (true)
    {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i)
        {
            uint64_t p = prime_list[i];
            if ((unsigned __int128)p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) break;
        candidate += 2;
    }

    if (prime_count == prime_capacity)
    {
        prime_capacity = prime_capacity ? prime_capacity * 2 : 16;
        prime_list = (uint64_t *)realloc(prime_list, prime_capacity * sizeof(uint64_t));
    }
    prime_list[prime_count++] = candidate;
}

/* ------------------------------------------------------------------ */
/* Make sure prime_list contains all primes <= sqrt(m).               */
/* ------------------------------------------------------------------ */
static void ensure_primes_up_to_sqrt(uint64_t m)
{
    if (prime_count == 0) return;

    while ((unsigned __int128)prime_list[prime_count - 1] *
                      prime_list[prime_count - 1] <= m)
    {
        generate_next_prime();
    }
}

/* ------------------------------------------------------------------ */
/* Test whether x is the product of exactly two distinct primes.      */
/* ------------------------------------------------------------------ */
static bool is_two_distinct_prime_product(uint64_t x)
{
    if (x < 6) return false;          /* smallest such product is 2*3 */

    ensure_primes_up_to_sqrt(x);

    /* Find the first prime factor p. */
    uint64_t p = 0;
    for (size_t i = 0; i < prime_count; ++i)
    {
        uint64_t prime = prime_list[i];
        if ((unsigned __int128)prime * prime > x) break;
        if (x % prime == 0)
        {
            p = prime;
            break;
        }
    }
    if (p == 0) return false; /* x itself is prime */

    uint64_t q = x / p;
    if (q == p) return false;                /* identical primes not allowed */

    /* q must be prime.  test by trial division using primes up to sqrt(q). */
    ensure_primes_up_to_sqrt(q);
    for (size_t i = 0; i < prime_count; ++i)
    {
        uint64_t prime = prime_list[i];
        if ((unsigned __int128)prime * prime > q) break;
        if (q % prime == 0) return false;
    }
    /* q > 1 and not divisible by any smaller prime -> prime. */
    return true;
}

/* ------------------------------------------------------------------ */
/* Generate terms until at least 'needed' are stored.                  */
/* ------------------------------------------------------------------ */
static void generate_terms(size_t needed)
{
    while (term_len < needed)
    {
        /* Ensure next pair of primes is available. */
        while (prime_count <= next_pair_index + 1)
            generate_next_prime();

        uint64_t p = prime_list[next_pair_index];
        uint64_t q = prime_list[next_pair_index + 1];

        /* (p+q) must be even for an integer half. */
        if (((p + q) & 1ULL) == 0)
        {
            uint64_t half = (p + q) >> 1;
            if (is_two_distinct_prime_product(half))
            {
                if (term_len == term_capacity)
                {
                    term_capacity = term_capacity ? term_capacity * 2 : 16;
                    term_list = (seq_t *)realloc(term_list,
                                                 term_capacity * sizeof(seq_t));
                }
                term_list[term_len++] = (seq_t)p;
            }
        }
        ++next_pair_index;
    }
}

/* ------------------------------------------------------------------ */
/* External sequence function.                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1 */

    size_t target = (size_t)n;

    ensure_initialized();

    if (term_len < target)
        generate_terms(target);

    return term_list[target - 1];
}
