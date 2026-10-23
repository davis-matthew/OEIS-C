/*
 * A141042  Product of n and the n-th gap between primes: a(n) = n*A001223(n).
 * https://oeis.org/A141042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(prime(n+1)-prime(n))
 * Range:     u8 n <= 29, u16 n <= 1830, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sieve_first_n_primes.h"   /* provides sieve_first_n_primes(idx_t n, uint32_t **out, size_t *cnt_out) */

static uint32_t *primes_u32 = NULL;
static size_t     primes_cnt = 0;

/* Ensure that primes_u32 holds at least needed primes (needed ≥ 1). */
static void
ensure_primes_upto(size_t needed)
{
    if (primes_cnt >= needed)
        return;

    /* Free any previously allocated array. */
    if (primes_u32) {
        free(primes_u32);
        primes_u32 = NULL;
    }

    /* Allocate a fresh array with the requested number of primes. */
    sieve_first_n_primes((idx_t)needed, &primes_u32, &primes_cnt);
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for n < 1 */

    /* We need prime(n) and prime(n+1). */
    ensure_primes_upto((size_t)n + 1);

    /* Convert the stored 32‑bit primes to seq_t for the arithmetic. */
    seq_t pn  = (seq_t)primes_u32[n - 1];   /* prime(n)   */
    seq_t pn1 = (seq_t)primes_u32[n];       /* prime(n+1) */

    seq_t diff = pn1 - pn;
    return (seq_t)n * diff;
}
