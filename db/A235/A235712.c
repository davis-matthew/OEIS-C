/*
 * A235712  Least prime p < prime(n) with 2^p + 1 a quadratic nonresidue
 *          modulo prime(n), or 0 if such a prime p does not exist.
 * https://oeis.org/A235712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p < prime(n) with 2^p + 1 a quadratic *
 *            nonresidue modulo prime(n), or 0 if no such prime exists. * This
 *            implementation generates primes up to the required bound * by a
 *            simple sieve and tests the quadratic residuosity using Euler's *
 *            criterion. No external helper functions are...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Simple modular exponentiation for 64‑bit values                      */
/* ------------------------------------------------------------------ */
static inline uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1;
    base %= mod;
    while (exp > 0)
    {
        if (exp & 1U)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1U;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Prime storage and sieve                                            */
/* ------------------------------------------------------------------ */
static idx_t *prime_array   = NULL;   /* prime list, 0‑based index */
static size_t primes_needed = 0;       /* number of primes stored      */

/* Generate all primes ≤ limit using a classic sieve of Eratosthenes.
 * The result is stored in 'prime_array' and its length in 'primes_needed'.
 * Any previously allocated prime list is freed.                         */
static void generate_primes_up_to(idx_t limit)
{
    /* Allocate a boolean array for the sieve */
    bool *is_composite = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (is_composite == NULL)
        return;                         /* allocation failed – leave list empty */

    /* Sieve */
    for (idx_t i = 2; (uint64_t)i * (uint64_t)i <= limit; ++i)
    {
        if (!is_composite[i])
        {
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;
        }
    }

    /* Count primes */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            ++count;

    /* Reallocate prime list */
    idx_t *new_array = (idx_t *)malloc(count * sizeof(idx_t));
    if (new_array == NULL)
    {
        free(is_composite);
        return;                         /* allocation failed – leave list empty */
    }

    /* Fill prime list */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            new_array[idx++] = i;

    /* Free old data and install new list */
    free(prime_array);
    free(is_composite);
    prime_array   = new_array;
    primes_needed = count;
}

/* Ensure that at least the first 'n' primes are available.
 * The required upper bound for the nth prime is roughly n*(log n + log log n).
 * For simplicity, we use n*20 + 100, which is sufficient for n up to 10000. */
static void ensure_prime_list(idx_t n)
{
    if (n <= primes_needed)
        return;                         /* we already have enough primes */

    /* Rough upper bound for the nth prime */
    idx_t limit;
    if (n < 6)
        limit = 15;
    else
        limit = n * 20 + 100;           /* safe for n ≤ 10000 */

    generate_primes_up_to(limit);
}

/* ------------------------------------------------------------------ */
/* Sequence function a(idx_t n)                                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* outside OEIS offset */

    /* Make sure we have enough primes */
    ensure_prime_list(n);

    if (n > primes_needed)              /* safety guard */
        return (seq_t)0;

    /* prime(n) */
    uint64_t prime_n = (uint64_t)prime_array[n - 1];

    /* Test primes p < prime(n) in increasing order */
    for (size_t i = 0; i < primes_needed; ++i)
    {
        uint64_t p = (uint64_t)prime_array[i];
        if (p >= prime_n)
            break;                     /* all remaining primes are ≥ prime_n */

        /* Compute (2^p + 1) mod prime_n */
        uint64_t val = modpow_uint64(2ULL, p, prime_n);
        val = (val + 1ULL) % prime_n;
        if (val == 0ULL)
            continue;                  /* trivial residue */

        /* Euler's criterion: val is a quadratic nonresidue iff
         * val^((prime_n-1)/2) ≡ prime_n-1 (mod prime_n).          */
        uint64_t legendre = modpow_uint64(val, (prime_n - 1U) / 2U, prime_n);
        if (legendre == prime_n - 1U)
            return (seq_t)p;            /* first suitable prime found */
    }

    return (seq_t)0;                     /* no nonresidue found */
}
