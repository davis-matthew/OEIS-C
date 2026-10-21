/*
 * A077218  Sum of numbers of prime factors (counted with multiplicities) of
 *          numbers between n-th and (n+1)-st prime.
 * https://oeis.org/A077218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=prime(n)+1}^{prime(n+1)-1} Ω(k) where Ω(k) is the
 *            number of prime factors of k counted with multiplicity
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation pre‑computes a linear sieve up to a fixed limit
   that comfortably exceeds the 10 000th prime (≈104 730).  The sieve
   supplies the smallest prime factor of every integer, from which Ω(k)
   is constructed, and a prefix sum of Ω(k).  The sequence term a(n)
   is then obtained in O(1) time from the prefix sums. */

#include <stdlib.h>
#include <stdint.h>

/* The build supplies the types seq_t (arbitrary‑precision) and idx_t
   (unsigned 64‑bit).  We only use standard C types for the sieve. */
static uint32_t *spf = NULL;      /* smallest prime factor of i */
static idx_t *primes = NULL;      /* list of primes, 1‑based in OEIS sense */
static seq_t *omega = NULL;       /* Ω(i) for each i */
static seq_t *pref = NULL;        /* prefix sums of Ω(i) */
static size_t sieve_limit = 0;
static size_t prime_count = 0;

/* Initialise the sieve once.  The limit is chosen to contain >150 000
   primes, enough for n up to 10 000 (prime 10 000 ≈ 104 729). */
static void ensure_initialized(void)
{
    if (sieve_limit != 0)
        return;   /* already initialised */

    /* Limit slightly above the 200 000th prime (≈3 042 011). */
    sieve_limit = 2000000;

    /* Allocate arrays. */
    spf   = (uint32_t *)malloc((sieve_limit + 1) * sizeof(uint32_t));
    omega = (seq_t   *)malloc((sieve_limit + 1) * sizeof(seq_t));
    pref  = (seq_t   *)malloc((sieve_limit + 1) * sizeof(seq_t));
    /* Approximate number of primes up to sieve_limit. */
    primes = (idx_t   *)malloc((sieve_limit / 10 + 10) * sizeof(idx_t));

    if (!spf || !omega || !pref || !primes) {
        abort();   /* memory allocation failed */
    }

    /* Linear sieve for spf and primes array. */
    for (size_t i = 0; i <= sieve_limit; ++i)
        spf[i] = 0;
    prime_count = 0;
    for (uint32_t i = 2; i <= sieve_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[prime_count++] = i;
        }
        for (size_t j = 0; j < prime_count; ++j) {
            uint32_t p = primes[j];
            if (p > spf[i] || (uint64_t)i * p > sieve_limit)
                break;
            spf[i * p] = p;
        }
    }

    /* Compute Ω(i). */
    omega[0] = 0;
    omega[1] = 0;
    for (uint32_t i = 2; i <= sieve_limit; ++i) {
        omega[i] = omega[i / spf[i]] + 1;
    }

    /* Prefix sums of Ω(i). */
    pref[0] = 0;
    for (uint32_t i = 1; i <= sieve_limit; ++i) {
        pref[i] = pref[i - 1] + omega[i];
    }
}

/* Return a(n) for n >= 1.  The function is safe for repeated calls in
   any order; it uses the precomputed data. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* sequence is undefined for n = 0 (offset 1) */

    ensure_initialized();

    /* Ensure we have the n-th and (n+1)-th primes. */
    if ((size_t)n + 1 > prime_count) {
        /* Input too large for the pre‑computed limit. */
        return 0;
    }

    /* 1‑based OEIS indices: primes[0] = 2 = prime(1). */
    idx_t p_n     = primes[n - 1];
    idx_t p_next  = primes[n];          /* (n+1)-th prime */

    /* Sum Ω(k) for p_n < k < p_next.  Indexes are 0‑based in C. */
    seq_t result = pref[p_next - 1] - pref[p_n];
    return result;
}
