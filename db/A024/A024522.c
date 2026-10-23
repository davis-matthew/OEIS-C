/*
 * A024522  a(n) = 2nd elementary symmetric function of {1, prime(1),
 *          prime(2), ..., prime(n-1)}, where prime(0) = 1.
 * https://oeis.org/A024522
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A024522
   a(n) = 2nd elementary symmetric function of
          {1, prime(1), prime(2), ..., prime(n-1)},
   with prime(0) = 1.
   Formula: a(n) = ((1 + Σ_{k=1}^{n-1} p_k)^2
                    - (1 + Σ_{k=1}^{n-1} p_k^2)) / 2
   where p_k is the k‑th prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------- static state ------------------------------------- */
static uint64_t *primes      = NULL;  /* list of primes, primes[0] = 2          */
static size_t   primes_len   = 0;     /* number of stored primes              */
static size_t   primes_cap   = 0;     /* capacity of primes array             */

static seq_t *prime_sum    = NULL;   /* prefix sums of primes, prime_sum[0]=0 */
static seq_t *prime_sq_sum = NULL;   /* prefix sums of prime squares         */
static size_t sums_len     = 0;       /* length of sums arrays (>=1)           */
static size_t sums_cap     = 0;       /* capacity of sums arrays              */

static uint64_t next_candidate = 2;   /* next integer to test for primality   */

/* --------- helper functions --------------------------------- */
/* Grow an array of uint64_t to at least 'needed' elements.      */
static void grow_primes(size_t needed)
{
    if (primes_cap < needed) {
        size_t new_cap = (primes_cap == 0) ? 16 : primes_cap;
        while (new_cap < needed) new_cap <<= 1;
        void *p = realloc(primes, new_cap * sizeof(uint64_t));
        primes      = (uint64_t *)p;
        primes_cap  = new_cap;
    }
}

/* Grow the sum arrays to at least 'needed' elements.           */
static void grow_sums(size_t needed)
{
    if (sums_cap < needed) {
        size_t new_cap = (sums_cap == 0) ? 16 : sums_cap;
        while (new_cap < needed) new_cap <<= 1;
        void *p1 = realloc(prime_sum,   new_cap * sizeof(seq_t));
        void *p2 = realloc(prime_sq_sum, new_cap * sizeof(seq_t));
        prime_sum   = (seq_t *)p1;
        prime_sq_sum= (seq_t *)p2;
        sums_cap    = new_cap;
    }
}

/* Simple trial division primality test using the primes we already know. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Add a newly found prime into the list and update prefix sums. */
static void add_prime(uint64_t p)
{
    /* Ensure capacity for the prime list. */
    grow_primes(primes_len + 1);

    /* Ensure capacity for the sum arrays.  Create the zero
       element lazily on first call. */
    if (sums_len == 0) {
        grow_sums(1);          /* allocate element 0 */
        prime_sum[0]    = (seq_t)0;
        prime_sq_sum[0] = (seq_t)0;
        sums_len = 1;
    }
    grow_sums(sums_len + 1);

    /* Update prefix sums. */
    prime_sum[sums_len]     = prime_sum[sums_len - 1] + (seq_t)p;
    prime_sq_sum[sums_len]  = prime_sq_sum[sums_len - 1]
                               + (seq_t)p * (seq_t)p;

    /* Store the prime. */
    primes[primes_len] = p;
    primes_len++;
    sums_len++;
}

/* Ensure that at least 'needed' primes are stored.  Needed >= 1. */
static void ensure_primes_count(size_t needed)
{
    while (primes_len < needed) {
        if (is_prime_uint64(next_candidate))
            add_prime(next_candidate);
        next_candidate++;
    }
}

/* --------- sequence function -------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0; /* undefined for n < 2 */

    size_t need_primes = (size_t)(n - 1); /* number of primes required */
    ensure_primes_count(need_primes);

    /* sum of the first (n‑1) primes and sum of squares */
    seq_t sum_primes    = prime_sum[need_primes];
    seq_t sum_sq_primes = prime_sq_sum[need_primes];

    /* include the leading 1 in the sums */
    seq_t s  = (seq_t)1 + sum_primes;           /* S   = Σ x_i */
    seq_t ss = (seq_t)1 + sum_sq_primes;        /* S₂  = Σ x_i² */

    return (s * s - ss) / (seq_t)2;
}
