/*
 * A071243  Record terms in A005179.
 * https://oeis.org/A071243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (n=1), 2 (n=2), 4 (n=3), 6 (n=4), a(n) = 2^(prime(n‑2)
 *            - 1) for n ≥ 5 (prime(k) = the k‑th prime)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   prime_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/prime_bound.h"

/* ---------- Prime generation ---------- */
static uint32_t *primes = NULL;          /* Array of primes */
static size_t   primes_cnt = 0;          /* Number of primes stored */

/* Generate primes up to the required count.  The array is rebuilt
   from scratch; this is fine for n ≤ ~10⁵. */
static void generate_primes_up_to(size_t need)
{
    uint32_t limit = prime_bound(need);
    uint8_t *sieve = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!sieve) return;

    for (uint32_t i = 0; i <= limit; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (uint32_t i = 2; i * i <= limit; ++i)
        if (sieve[i])
            for (uint32_t j = i * i; j <= limit; j += i)
                sieve[j] = 0;

    /* Count primes and allocate array */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++cnt;

    uint32_t *newpr = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!newpr) { free(sieve); return; }

    size_t idx = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (sieve[i]) newpr[idx++] = i;

    free(sieve);
    if (primes) free(primes);
    primes = newpr;
    primes_cnt = cnt;
}

/* ---------- Power of two ---------- */
static seq_t pow2(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i) r *= 2;
    return r;
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)6;

    idx_t prime_index = n - 2;                     /* 1‑based */
    if ((size_t)prime_index > primes_cnt)
        generate_primes_up_to((size_t)prime_index);

    uint32_t p = primes[prime_index - 1];          /* p = prime(n‑2) */
    idx_t exp = (idx_t)(p - 1);
    return pow2(exp);
}
