/*
 * A230780  Positive numbers without a prime factor congruent to 1 (mod 6).
 * https://oeis.org/A230780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A230780.
   a(n) is the n-th positive integer that has no prime factor congruent
   to 1 modulo 6.  The sequence is closed under multiplication; its
   primitive elements are 3 and all primes ≡ 2 or 5 (mod 6). */

/* ----------------------------------------------------------- */
/*  Standard headers needed for memory allocation and types.   */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ----------------------------------------------------------- */
/*  Prime sieve up to a fixed limit.  The limit is chosen so
   that the 10 000th term of the sequence lies well below the
   square of the largest prime considered (this is true for all
   known n ≤ 10 000, and guarantees correct behaviour for the
   required test limits). */
static const size_t PRIME_LIMIT = 2000000;      /* 2 000 000 → 78 498 primes */
static uint64_t *prime_arr   = NULL;            /* array of primes */
static size_t   prime_cnt   = 0;                /* number of primes */

/* Initialize the prime table once. */
static void init_primes(void)
{
    if (prime_arr != NULL)
        return;                                 /* already ready */

    /* Allocate temporary sieve array. */
    bool *sieve = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!sieve)
        exit(1);

    sieve[0] = sieve[1] = true;                 /* 0 and 1 are non‑primes */

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!sieve[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = true;

    /* Count primes. */
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i])
            ++prime_cnt;

    /* Allocate prime array. */
    prime_arr = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (!prime_arr)
        exit(1);

    /* Store primes. */
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i])
            prime_arr[idx++] = (uint64_t)i;

    free(sieve);                                 /* sieve no longer needed */
}

/* ----------------------------------------------------------- */
/*  Return true if n has any prime factor ≡ 1 (mod 6).  The
   argument n may be of arbitrary size; comparisons and
   modulo operations are performed with seq_t operators. */
static bool has_banned_prime_factor(seq_t n)
{
    if (n <= (seq_t)1)
        return false;                             /* 1 has no prime factors */

    seq_t temp = n;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        seq_t sq = (seq_t)p * (seq_t)p;
        if (sq > temp)
            break;

        if (temp % (seq_t)p == (seq_t)0) {         /* p divides temp */
            if ((p % 6) == 1)                    /* banned prime found */
                return true;
            /* Remove all powers of p. */
            do
                temp /= (seq_t)p;
            while (temp % (seq_t)p == (seq_t)0);
        }
    }

    /* Any remaining factor > 1 is prime > sqrt(original). */
    if (temp > (seq_t)1)
        return ((temp % (seq_t)6) == (seq_t)1);

    return false;
}

/* ----------------------------------------------------------- */
/*  Cache for the sequence values. */
static seq_t *cache      = NULL;                  /* array of seq_t values */
static size_t cache_cap  = 0;                     /* allocated capacity */
static idx_t  cache_size = 0;                     /* number of values stored */

/* Ensure cache contains at least the first n terms (n ≥ 1). */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n)
        return;

    init_primes();                                 /* prime table ready */

    seq_t candidate = (cache_size == 0) ? (seq_t)1
                                        : cache[cache_size - 1] + (seq_t)1;

    for (idx_t count = cache_size + 1; count <= n; ++count) {
        /* Skip values that have a banned prime factor. */
        while (has_banned_prime_factor(candidate))
            candidate += (seq_t)1;

        /* Grow cache if necessary. */
        if (cache_size + 1 > cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_size++] = candidate;
        candidate += (seq_t)1;
    }
}

/* ----------------------------------------------------------- */
/*  The required function.  Indexing follows OEIS: a(1) is the
   first term listed.  For n == 0 the function returns 0
   (undefined but guards against accidental use). */
seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;
    ensure_cache(n);
    return cache[n - 1];
}
