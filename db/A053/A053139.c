/*
 * A053139  a(n) = phi(n) - mu(n).
 * https://oeis.org/A053139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(n) – μ(n) (Euler's totient minus the Möbius function)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine factors n by trial division using a small list of primes that is
   generated on demand.  It works for arbitrary‑precision seq_t as well as
   for built‑in unsigned integer types. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --- Global state for prime tables ------------------------------------- */
static uint32_t *primes = NULL;     /* array of prime numbers */
static size_t prime_cnt = 0;        /* number of primes stored */
static size_t sieve_limit = 0;      /* largest n for which primes have been generated */

/* Generate all primes ≤ limit and store them in the static array.          */
/* This routine may repeatedly be called with a larger limit to grow the    */
/* table.  The implementation is a simple Eratosthenes sieve.             */
static void ensure_primes(size_t limit)
{
    if (limit <= sieve_limit)            /* already enough primes */
        return;

    /* Allocate a new buffer with a generous capacity.  We'll grow it as
       necessary while filling it with primes. */
    size_t capacity = (limit > 16) ? limit / 2 : 16;
    uint32_t *buf = (uint32_t *)realloc(primes, capacity * sizeof(uint32_t));
    if (!buf)
        abort();                         /* allocation failure */
    primes = buf;
    prime_cnt = 0;

    /* Simple sieve of Eratosthenes up to 'limit'.  We use a separate
       array of flags to mark composites. */
    bool *composite = (bool *)calloc(limit + 1, sizeof(bool));
    if (!composite)
        abort();

    for (size_t i = 2; i <= limit; ++i) {
        if (!composite[i]) {
            /* Grow the prime buffer if needed. */
            if (prime_cnt >= capacity) {
                capacity *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, capacity * sizeof(uint32_t));
                if (!tmp)
                    abort();
                primes = tmp;
            }
            primes[prime_cnt++] = (uint32_t)i;

            /* Mark multiples of i as composite. */
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    composite[j] = true;
            }
        }
    }

    free(composite);
    sieve_limit = limit;
}

/* Compute a(n) = φ(n) – μ(n) for n ≥ 1.  The function accepts any
   idx_t value (usually uint64_t).  It is safe for n = 0 (undefined for
   the sequence) and returns 0 in that case. */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined term, return safe value */
        return (seq_t)0;

    /* Compute an integer bound for √n to know how far we must generate
       primes.  For n up to about 10000 a simple integer loop suffices. */
    size_t sq = 1;
    while ((sq + 1) * (sq + 1) <= n)
        ++sq;
    ensure_primes(sq);

    /* Factorise n.  tempU holds the remaining part of n during the loop. */
    unsigned long long tempU = (unsigned long long)n;
    seq_t phi = (seq_t)n;      /* Euler's totient, updated during factorisation */
    int mu = 1;                /* Möbius function:  1, -1, or 0 */

    for (size_t i = 0; i < prime_cnt && (unsigned long long)primes[i] * primes[i] <= tempU; ++i) {
        uint32_t p = primes[i];
        if (tempU % p == 0) {
            int power = 0;
            while (tempU % p == 0) {
                tempU /= p;
                ++power;
            }
            /* Update φ(n) ----------------- */
            phi -= phi / (seq_t)p;
            /* Update μ(n) ----------------- */
            if (power > 1)
                mu = 0;          /* squared prime factor */
            else
                mu = -mu;        /* flip sign for each distinct prime */
        }
    }

    /* If tempU > 1, the remaining factor is a prime > √original_n. */
    if (tempU > 1) {
        phi -= phi / (seq_t)tempU;
        mu = -mu;
    }

    /* Return φ(n) – μ(n). */
    return phi - (seq_t)mu;
}
