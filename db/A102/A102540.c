/*
 * A102540  Primes that are not Chen primes (see A109611).
 * https://oeis.org/A102540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A102540: Primes that are not Chen primes
   a(n) = the nth prime p such that p+2 is neither prime nor semiprime
   (i.e., not prime and has at least three prime factors, counting multiplicities)   */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/is_prime_uint64.h"

/* --------------------------------------------------------------------- */
/* Prime generation utilities                                           */
/* --------------------------------------------------------------------- */

/* dynamic arrays to store primes and the sought sequence values          */
static uint64_t *prime_arr       = NULL;
static size_t    prime_cnt       = 0;
static size_t    prime_cap       = 0;
static uint64_t *seq_arr         = NULL;   /* caches p where p+2 not a Chen prime */
static size_t    seq_cnt         = 0;
static size_t    seq_cap         = 0;

/* ensure we have room for at least `need` primes                        */
static void ensure_prime_capacity(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 256;
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_arr  = tmp;
    prime_cap  = new_cap;
}

/* ensure we have room for at least `need` sequence entries             */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 256;
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(seq_arr, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    seq_arr  = tmp;
    seq_cap  = new_cap;
}

/* generate primes incrementally, filling prime_arr and returning the next prime */
static uint64_t next_prime(void)
{
    static uint64_t last = 1;   /* last prime returned                                 */
    for (;;)
    {
        ++last;
        if (last == 2)        /* first prime */
        {
            ensure_prime_capacity(prime_cnt + 1);
            prime_arr[prime_cnt++] = 2;
            return 2;
        }
        if (last % 2 == 0) continue;
        if (is_prime_uint64(last))
        {
            ensure_prime_capacity(prime_cnt + 1);
            prime_arr[prime_cnt++] = last;
            return last;
        }
    }
}

/* make sure prime_arr contains all primes up to at least `limit`        */
static void ensure_primes_up_to(uint64_t limit)
{
    if (prime_cnt == 0) return;
    while (prime_arr[prime_cnt - 1] < limit) next_prime();
}

/* --------------------------------------------------------------------- */
/* Helper to decide whether `x` is a semiprime (product of two primes,
 *   including the square of a prime).                                   */
/* --------------------------------------------------------------------- */
static bool is_semiprime(uint64_t x)
{
    if (x < 4) return false;  /* 1,2,3 cannot be semiprime */
    uint64_t temp = x;
    int cnt = 0;

    /* we only need primes up to sqrt(x) */
    uint64_t limit = (uint64_t)sqrt((long double)temp) + 1;
    ensure_primes_up_to(limit);

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_arr[i];
        if (p > temp / p) break;      /* p^2 > temp, no further factorization */
        while (temp % p == 0)
        {
            temp /= p;
            ++cnt;
            if (cnt > 2) return false;  /* more than two prime factors   */
        }
    }
    if (temp > 1) ++cnt;    /* remaining prime factor > sqrt(x)            */
    return cnt == 2;
}

/* --------------------------------------------------------------------- */
/* The required sequence function                                       */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* the sequence is defined only for n ≥ 1 */

    /* Grow cache if necessary */
    if ((size_t)n > seq_cnt)
    {
        ensure_seq_capacity((size_t)n);
        while (seq_cnt < (size_t)n)
        {
            uint64_t p = next_prime();           /* candidate prime       */
            uint64_t q = p + 2;                  /* p+2 to test           */
            if (is_prime_uint64(q))              /* q is prime → Chen prime: skip */
                continue;
            if (is_semiprime(q))                 /* q is semiprime → Chen prime: skip */
                continue;
            seq_arr[seq_cnt++] = p;              /* p is not a Chen prime */
        }
    }

    return (seq_t)seq_arr[n - 1];
}
