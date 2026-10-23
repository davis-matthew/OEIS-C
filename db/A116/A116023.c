/*
 * A116023  The n-th prime plus n gives a semiprime (A001358).
 * https://oeis.org/A116023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A116023: a(n) is the n‑th integer m for which the
   m‑th prime p(m) plus m is a semiprime (product of exactly two primes). */

/* The code generates primes incrementally, tests each p(m)+m
   for semiprimality by trial division using the primes found so far,
   and stores the resulting indices m in a cache.  The implementation
   supports arbitrary calls to a(n) in any order and is efficient
   for n up to about 10 000. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*--------- Prime generation helpers -------------------------------------*/
/* Dynamically grow a list of primes stored as uint64_t.  The first prime
   stored is 2.  prime_next_candidate starts at 2 and is incremented
   after each trial. */
static uint64_t *prime_list   = NULL;
static size_t   prime_cap    = 0;
static size_t   prime_len    = 0;
static uint64_t prime_next    = 2;

/* Ensure the prime list contains at least 'needed' primes (1‑based). */
static void
ensure_prime_count(size_t needed)
{
    while (prime_len < needed) {
        uint64_t cand = prime_next++;
        bool is_prime = true;

        /* trial division by previously found primes */
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_list[i];
            if (p > cand / p)            /* p*p > cand – safe from overflow */
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (prime_len == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    new_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);   /* out of memory */
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_len++] = cand;
        }
    }
}

/* Return the m‑th prime (1‑based).  Assumes 1 ≤ m. */
static uint64_t
nth_prime(idx_t m)
{
    ensure_prime_count((size_t)m);
    return prime_list[(size_t)(m - 1)];
}

/*--------- Semiprimality test -------------------------------------------*/
/* Test whether x is a product of exactly two prime factors
   (they may be equal).  x is assumed to be >= 4. */
static bool
is_semiprime(uint64_t x)
{
    if (x < 4) return false;              /* 1,2,3 are not semiprimes */

    uint64_t factor1 = 0;
    for (uint64_t i = 2; i <= x / i; ++i) {  /* i*i <= x safe from overflow */
        if (x % i == 0) { factor1 = i; break; }
    }
    if (factor1 == 0) return false;        /* x is prime */

    uint64_t factor2 = x / factor1;

    /* check that factor2 is prime */
    for (uint64_t i = 2; i <= factor2 / i; ++i) {
        if (factor2 % i == 0) return false;   /* composite -> not semiprime */
    }
    return true;
}

/*--------- Sequence cache -----------------------------------------------*/
/* Cache of terms: seq_cache[k] holds the (k+1)‑th sequence value,
   i.e., the index m where p(m)+m is semiprime. */
static uint64_t *seq_cache = NULL;
static size_t   seq_cap   = 0;
static size_t   seq_len   = 0;
static idx_t    next_check = 1;           /* next m to test for property */

/* Expand the cache until it contains at least 'needed' terms. */
static void
extend_cache(idx_t needed)
{
    while (seq_len < (size_t)needed) {
        uint64_t m   = (uint64_t)next_check;
        uint64_t p   = nth_prime(m);
        uint64_t val = p + m;

        if (is_semiprime(val)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(seq_cache,
                                                    new_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                seq_cache = tmp;
                seq_cap   = new_cap;
            }
            seq_cache[seq_len++] = m;
        }
        ++next_check;
    }
}

/*--------- Public sequence function --------------------------------------*/
seq_t
a(idx_t n)
{
    if (n == 0) return 0;               /* undefined below offset; safe fallback */
    extend_cache(n);
    return (seq_t)seq_cache[(size_t)(n - 1)];
}
