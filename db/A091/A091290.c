/*
 * A091290  Numbers k such that sigma(k) + sigma(k+1) <= sigma(2*k+1).
 * https://oeis.org/A091290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 2985, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that sigma(k) + sigma(k+1) <= sigma(2*k+1). */
/* The sequence is built by testing k in increasing order and keeping the
   found values.  Primes are generated incrementally and used for sigma
   computations. */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* ---------- prime generation ---------- */
static uint64_t *prime_list = NULL;     /* list of primes */
static size_t   prime_len  = 0;         /* number of primes stored   */
static size_t   prime_cap  = 0;         /* allocated capacity        */
static uint64_t prime_limit = 1;        /* largest prime generated   */

static void grow_primes_to(uint64_t limit)
{
    if (limit <= prime_limit)
        return;

    if (prime_len == 0) {            /* initialise with prime 2   */
        prime_cap = 4;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_list[prime_len++] = 2;
        prime_limit = 2;
    }

    for (uint64_t cand = prime_limit + 1; cand <= limit; ++cand) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            if (prime_len == prime_cap) {
                prime_cap <<= 1;
                prime_list = (uint64_t *)realloc(prime_list,
                                        prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_len++] = cand;
        }
    }
    prime_limit = limit;
}

/* ---------- sigma(n) ---------- */
static seq_t sigma_of_uint64(uint64_t n)
{
    uint64_t x = n;
    unsigned __int128 sum = 1;

    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) {
            uint64_t exp = 0;
            while (x % p == 0) { x /= p; ++exp; }
            unsigned __int128 pow = 1;
            for (uint64_t j = 0; j < exp + 1; ++j) pow *= p;
            unsigned __int128 term = (pow - 1) / (p - 1);
            sum *= term;
        }
    }
    if (x > 1) {                 /* remaining prime factor */
        sum *= (unsigned __int128)(x + 1);
    }
    return (seq_t)sum;
}

/* ---------- cache of sequence values ---------- */
static seq_t *seq_cache    = NULL;  /* stored k values */
static size_t seq_len     = 0;
static size_t seq_cap     = 0;
static uint64_t cur_k     = 1;      /* next k to test */

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* sequence is defined for n >= 1 */

    /* initialise storage if needed */
    if (seq_cache == NULL) {
        seq_cap   = 64;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    /* extend the sequence until it contains the requested term */
    while (seq_len < (size_t)n) {
        uint64_t k   = cur_k++;
        uint64_t v1  = k;
        uint64_t v2  = k + 1;
        uint64_t v3  = 2 * k + 1;

        /* make sure we have enough primes for the largest argument */
        grow_primes_to(v3);

        seq_t s1 = sigma_of_uint64(v1);
        seq_t s2 = sigma_of_uint64(v2);
        seq_t s3 = sigma_of_uint64(v3);

        if ((unsigned __int128)s1 + s2 <= s3) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq_cache = (seq_t *)realloc(seq_cache,
                                seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)k;
        }
    }

    return seq_cache[n - 1];
}
