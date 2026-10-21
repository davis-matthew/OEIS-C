/*
 * A056607  a(n) is the n-th primorial divided by squarefree kernel of
 *          corresponding central binomial coefficient.
 * https://oeis.org/A056607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002110(n) / A048633(n) = product of first n primes *
 *            that do NOT divide the central binomial coefficient C(n,⌊n/2⌋).
 *            * This implementation pre‑computes primes on demand with a
 *            simple * sieve and evaluates the exponent of each prime in the
 *            binomial * coefficient by Legendre’s formula
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- prime storage ---------- */
static size_t sieve_limit = 0;         /* highest number currently sieved */
static size_t prime_count = 0;         /* total primes found          */
static uint32_t *primes = NULL;        /* array of primes             */
static size_t primes_cap = 0;          /* allocated capacity of array  */
static unsigned char *is_comp = NULL;  /* sieve flag array            */
static size_t comp_cap = 0;            /* capacity of sieve array      */

/* Rebuild all primes up to limit.  Uses realloc; casts for C++ compatibility. */
static void sieve_up_to(size_t limit)
{
    if (limit + 1 > comp_cap) {
        unsigned char *tmp = (unsigned char *)realloc(is_comp, limit + 1);
        if (!tmp) return;
        is_comp = tmp;
        comp_cap = limit + 1;
    }
    memset(is_comp, 0, limit + 1);

    for (size_t i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;

    /* Build the prime list */
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) {
            if (idx >= primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 1024;
                uint32_t *tmpP = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
                if (!tmpP) return;
                primes = tmpP;
                primes_cap = new_cap;
            }
            primes[idx++] = (uint32_t)i;
        }
    prime_count = idx;
    sieve_limit = limit;
}

/* Ensure we have at least N primes.  Expand limit until we do. */
static void ensure_prime_count(size_t N)
{
    if (prime_count >= N) return;
    size_t limit = sieve_limit ? sieve_limit : 2;
    while (prime_count < N) {
        limit <<= 1;               /* double the bound */
        sieve_up_to(limit);
        if (prime_count >= N) break;
    }
}

/* Legendre's formula: exponent of prime p in m! */
static seq_t v_p_factorial(idx_t m, uint32_t p)
{
    seq_t res = 0;
    idx_t d = m / p;
    while (d) {
        res += d;
        d /= p;
    }
    return res;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* safeguard: offset is 1, but handle 0 gracefully */
    size_t N = (size_t)n;                     /* needs N primes */
    ensure_prime_count(N);

    /* k = floor(n/2), nk = n - k */
    idx_t k  = n / 2;
    idx_t nk = n - k;

    seq_t result = 1;
    /* Only the first N primes are relevant (primorial). */
    for (size_t i = 0; i < N; ++i) {
        uint32_t p = primes[i];
        seq_t vp = v_p_factorial(n, p)
                 - v_p_factorial(k, p)
                 - v_p_factorial(nk, p);
        if (vp == 0)           /* p does NOT divide the central binomial */
            result *= (seq_t)p;
    }
    return result;
}
