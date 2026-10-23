/*
 * A181670  Triangle read by rows: T(n,k) = 2^(n-1) mod prime(k), 1 <= k <= n.
 * https://oeis.org/A181670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the row‑wise read of the triangle T(n,k) = 2^(n‑1) mod
 *            prime(k), 1 ≤ k ≤ n (prime(k) is the k‑th prime, starting with
 *            2)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   mod_pow, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_pow.h"
#include "../utils/sieve_first_n_primes.h"

static uint32_t *cached_primes = NULL; /* array of first primes[0..cnt-1] */
static size_t   cached_primes_cnt = 0;

/* Ensure that cached_primes contains at least 'needed' primes. */
static void
ensure_primes(size_t needed)
{
    if (needed <= cached_primes_cnt)
        return;                       /* already have enough */

    /* free old array */
    if (cached_primes)
        free(cached_primes);

    uint32_t *p = NULL;
    size_t    pc = 0;
    sieve_first_n_primes(needed, &p, &pc);

    cached_primes     = p;
    cached_primes_cnt = pc;
}

seq_t
a(idx_t n)
{
    if (n == 0)               /* not part of the sequence, but safe */
        return 0;

    /* find the triangle row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* index of column k within this row (1‑based) */
    idx_t k = n - ((r - 1) * r) / 2;

    /* make sure we have enough primes to address prime(k) */
    ensure_primes(r);

    uint32_t prime_k = cached_primes[k - 1];   /* k‑th prime */

    /* compute 2^(r-1) mod prime_k */
    seq_t result = mod_pow((seq_t)2, (uint64_t)(r - 1), (seq_t)prime_k);

    return result;
}
