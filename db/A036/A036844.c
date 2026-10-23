/*
 * A036844  Numbers k such that k / sopfr(k) is an integer, where sopfr =
 *          sum-of-prime-factors, A001414.
 * https://oeis.org/A036844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 7264, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A036844: numbers k such that
   k / sopfr(k) is an integer.  The implementation searches for
   qualifying numbers sequentially and memoises the results so that
   any order of calls to a(idx_t n) works efficiently. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"

/* ------------------------------------------------------------------ */
/*  Static tables and helpers                                            */
/* ------------------------------------------------------------------ */
static seq_t *seq      = NULL;  /* memoised sequence values          */
static size_t seq_len  = 0;     /* number of values stored           */
static size_t seq_cap  = 0;     /* current capacity of seq array      */

static uint32_t *primes    = NULL;   /* prime list up to primes_limit     */
static size_t primes_count = 0;
static size_t primes_limit   = 0;

/* Ensure that primes[] contains all primes ≤ limit.  The function
   rebuilds the sieve from scratch, which is adequate for the size
   of n considered (≤10 000). */
static void
ensure_primes_up_to(size_t limit)
{
    if (limit <= primes_limit) return;

    /* Sieve of Eratosthenes up to 'limit'. */
    size_t sieve_size = limit + 1;
    char *isprime = (char *)calloc(sieve_size, 1);
    if (!isprime) abort();

    for (size_t i = 2; i <= limit; ++i) isprime[i] = 1;

    for (size_t p = 2; p * p <= limit; ++p) {
        if (isprime[p]) {
            for (size_t j = p * p; j <= limit; j += p)
                isprime[j] = 0;
        }
    }

    /* Count primes for allocation. */
    size_t new_count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (isprime[i]) ++new_count;

    primes = (uint32_t *)realloc(primes, new_count * sizeof(uint32_t));
    if (!primes) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (isprime[i]) primes[idx++] = (uint32_t)i;

    free(isprime);

    primes_count = new_count;
    primes_limit = limit;
}

/* Append a new value to the memoised sequence, resizing as necessary. */
static void
seq_append(seq_t value)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!seq) abort();
        seq_cap = new_cap;
    }
    seq[seq_len++] = value;
}

/* Sum of prime factors with multiplicity for k (k ≥ 2).  The function
   uses the primes table built by ensure_primes_up_to. */
static seq_t
sum_prime_factors(idx_t k)
{
    seq_t sum = 0;
    uint64_t temp = k;

    for (size_t i = 0; i < primes_count &&
                     (uint64_t)primes[i] * primes[i] <= temp; ++i)
    {
        uint32_t p = primes[i];
        while (temp % p == 0) {
            sum += p;
            temp /= p;
        }
    }
    if (temp > 1) sum += temp;   /* remaining prime factor */
    return sum;
}

/* ------------------------------------------------------------------ */
/*  The required function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but safe */

    static idx_t last_checked_k = 1;    /* largest k that has been examined */

    while (seq_len < n) {
        ++last_checked_k;

        /* Ensure we have primes up to √last_checked_k. */
        size_t limit = _floor_isqrt_u64(last_checked_k) + 1;
        ensure_primes_up_to(limit);

        seq_t sopfr = sum_prime_factors(last_checked_k);

        if (sopfr != 0) {
            idx_t sop = (idx_t)sopfr;
            if (last_checked_k % sop == 0)
                seq_append(last_checked_k);
        }
    }
    return seq[n - 1];
}
