/*
 * A071636  Numbers n=4*k+2 such that there is no pair of primes p and q of
 *          form 4*i+1 with p+q=n; A071635(n)=0.
 * https://oeis.org/A071636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A071636: Numbers n = 4*k+2 such that there is no pair of
 * primes p and q of the form 4*i+1 with p+q = n.
 * The function a(idx_t n) returns the n-th term (n >= 1).
 * It is computed by iterating over the candidates n = 2,6,10,...,
 * using a sieve to test primality and checking all primes ≡ 1 (mod 4). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static state for caching the results */
static seq_t *terms = NULL;          /* terms[n-1] holds a(n) */
static size_t terms_len = 0;         /* number of computed terms */
static size_t terms_cap = 0;         /* allocated capacity for terms */

static uint64_t *prime_list = NULL;  /* all primes up to current sieve limit */
static size_t prime_cnt = 0;
static size_t prime_cap = 0;

static uint64_t *prime1mod4 = NULL;  /* primes ≡ 1 (mod 4) */
static size_t prime1mod4_cnt = 0;
static size_t prime1mod4_cap = 0;

static bool *is_prime = NULL;        /* boolean sieve array */
static size_t sieve_limit = 0;       /* current maximum sieved value */

static uint64_t next_candidate = 2;  /* next n = 4*k+2 to test */

/* ----------------------------------------------------------------------- */
/* | Utility: reallocate array with growth                                       */
/* ----------------------------------------------------------------------- */
static void *grow(void *ptr, size_t *cap, size_t need, size_t elem_size)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        void *tmp = realloc(ptr, new_cap * elem_size);
        if (!tmp) exit(1);            /* memory exhaustion */
        ptr = tmp;
        *cap = new_cap;
    }
    return ptr;
}

/* ----------------------------------------------------------------------- */
/* | Ensure sieve up to 'limit' and rebuild prime tables.                     */
/* ----------------------------------------------------------------------- */
static void ensure_sieve(size_t limit)
{
    if (limit <= sieve_limit) return;   /* already enough */

    /* Allocate or resize prime sieve */
    is_prime = (bool *)grow(is_prime, &sieve_limit, limit + 1, sizeof(bool));
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    /* Eratosthenes */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Build full prime list */
    prime_list = (uint64_t *)grow(prime_list, &prime_cap, limit / 10 + 10,
                                   sizeof(uint64_t));
    prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (prime_cnt == prime_cap)
                prime_list = (uint64_t *)grow(prime_list, &prime_cap,
                                              prime_cap * 2, sizeof(uint64_t));
            prime_list[prime_cnt++] = i;
        }
    }

    /* Build list of primes ≡ 1 (mod 4) */
    prime1mod4 = (uint64_t *)grow(prime1mod4, &prime1mod4_cap,
                                 prime_cnt / 2 + 10, sizeof(uint64_t));
    prime1mod4_cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if ((p & 3) == 1) {            /* check modulo 4 */
            if (prime1mod4_cnt == prime1mod4_cap)
                prime1mod4 = (uint64_t *)grow(prime1mod4, &prime1mod4_cap,
                                              prime1mod4_cap * 2, sizeof(uint64_t));
            prime1mod4[prime1mod4_cnt++] = p;
        }
    }

    sieve_limit = limit;
}

/* ----------------------------------------------------------------------- */
/* | Check if candidate x has a representation as sum of two primes         */
/* | ≡ 1 (mod 4).  Assumes sieve_limit ≥ x.                                  */
/* ----------------------------------------------------------------------- */
static bool has_representation(uint64_t x)
{
    for (size_t i = 0; i < prime1mod4_cnt; ++i) {
        uint64_t p = prime1mod4[i];
        if (p > x / 2) break;          /* p must be ≤ x/2 */
        uint64_t q = x - p;
        if (q <= sieve_limit && is_prime[q] && ((q & 3) == 1))
            return true;
    }
    return false;
}

/* ----------------------------------------------------------------------- */
/* | Main sequence generator                                                */
/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* the sequence is 1‑based */

    /* Ensure we have enough terms cached */
    while (terms_len < n) {
        uint64_t candidate = next_candidate;
        ensure_sieve(candidate);              /* we need primes up to candidate */

        if (!has_representation(candidate)) {
            /* Store the term */
            seq_t val = (seq_t)candidate;
            terms = (seq_t *)grow(terms, &terms_cap, terms_len + 1,
                                  sizeof(seq_t));
            terms[terms_len++] = val;
        }
        next_candidate += 4;                /* next number of form 4k+2 */
    }
    return terms[n - 1];
}
