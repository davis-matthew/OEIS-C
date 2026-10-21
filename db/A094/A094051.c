/*
 * A094051  a(n) = phi(phi(p))/2 where p = prime(n).
 * https://oeis.org/A094051
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(phi(p)) / 2 where p = prime(n) (offset = 3)
 * Range:     u8 n <= 54, u16 n <= 6989, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/phi.h"   /* seq_t phi(idx_t m); */

static idx_t *prime_list = NULL;   /* 1‑based array of primes */
static size_t prime_count = 0;     /* number of primes stored */

/* Simple sieve up to 200 000 – it contains >10 000 primes.  */
static void init_prime_sieve(void)
{
    if (prime_list != NULL)
        return;                     /* already initialized */

    const idx_t LIMIT = 200000;
    /* Allocate array to mark primes (1 = prime, 0 = composite).  */
    uint8_t *sieve = (uint8_t*)calloc((size_t)(LIMIT + 1), sizeof(uint8_t));
    if (!sieve)
        return;                     /* unlikely, but keep safe */

    for (idx_t i = 2; i <= LIMIT; ++i)
        sieve[i] = 1;

    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (sieve[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = 0;

    /* Count primes to size the list. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (sieve[i])
            ++cnt;

    prime_list = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!prime_list) {
        free(sieve);
        return;                     /* memory failure – keep safe */
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (sieve[i])
            prime_list[idx++] = i;

    prime_count = cnt;
    free(sieve);
}

/* Return the nth prime (1‑based).  Caller guarantees n‑1 < prime_count. */
static inline idx_t nth_prime(idx_t n)
{
    return prime_list[n - 1];
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                  /* undefined below offset, return 0 */

    init_prime_sieve();
    if (n - 1 >= prime_count)
        return 0;                  /* should not happen for intended range */

    idx_t p = nth_prime(n);        /* p = prime(n) */

    seq_t phi1 = phi(p);           /* φ(p) = p-1 for prime p (seq_t) */
    seq_t phi2 = phi((idx_t)phi1); /* φ(φ(p)) – cast back to idx_t */

    return phi2 / 2;               /* integer division, φ(φ(p)) is even */
}
