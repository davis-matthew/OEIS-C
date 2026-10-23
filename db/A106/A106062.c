/*
 * A106062  Primes p such that 1*p + 12 and 12*p + 1 are primes.
 * https://oeis.org/A106062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Primes p such that p + 12 and 12p + 1 are also prime.
 * The routine generates primes by an incremental segmented sieve
 * and tests the two additional congruence conditions with the
 * deterministic 64‑bit Miller–Rabin routine provided in
 * "../utils/is_prime.h".
 * For n ≤ 10000 the search finishes well under the time limit.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* ---- a small base‑prime generator (up to sqrt(MAX)) ---- */
static uint64_t *base_primes = NULL;
static uint64_t  base_prime_cnt = 0;

static void init_base_primes(uint64_t limit)
{
    if (base_primes) return;              /* already initialised */

    size_t size = (size_t)(limit + 1);
    bool *mark = (bool *)calloc(size, sizeof(bool));
    if (!mark) abort();

    for (uint64_t p = 2; p * p <= limit; ++p)
        if (!mark[p])
            for (uint64_t j = p * p; j <= limit; j += p)
                mark[j] = true;

    /* count primes */
    for (uint64_t i = 2; i <= limit; ++i)
        if (!mark[i]) ++base_prime_cnt;

    base_primes = (uint64_t *)malloc(base_prime_cnt * sizeof(uint64_t));
    if (!base_primes) abort();

    uint64_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!mark[i]) base_primes[idx++] = i;

    free(mark);
}

/* ----------------- main sequence function ----------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* behaviour for n<1 is undefined */

    /* limit for the sieving range – already comfortably above the 10 000th term */
    const uint64_t MAX_VALUE   = 4000000000ULL;   /* 4·10⁹  <  sqrt limit ²   */
    const uint64_t BLOCK_SIZE  = 1 << 20;        /* 1 048 576 numbers per block     */

    init_base_primes(70000);                    /* sqrt(MAX_VALUE) ≈ 63245 */

    uint64_t count = 0;
    uint64_t low   = 5;                          /* smallest candidate in the sequence */

    while (low <= MAX_VALUE)
    {
        uint64_t high = low + BLOCK_SIZE - 1;
        if (high > MAX_VALUE) high = MAX_VALUE;
        uint64_t sz = high - low + 1;

        bool *segment = (bool *)calloc(sz, sizeof(bool));
        if (!segment) abort();

        /* segmented sieve using precomputed small primes */
        for (uint64_t pi = 0; pi < base_prime_cnt; ++pi)
        {
            uint64_t p = base_primes[pi];
            if (p * p > high) break;

            uint64_t start = (low + p - 1) / p * p;   /* first multiple ≥ low */
            if (start < p * p) start = p * p;

            for (uint64_t j = start; j <= high; j += p)
                segment[j - low] = true;
        }

        /* search for candidates inside the segment */
        for (uint64_t offset = 0; offset < sz; ++offset)
        {
            uint64_t p = low + offset;
            if (p < 5) continue;                 /* skip numbers < 5 */
            if (segment[offset]) continue;        /* composite */

            /* p is prime – check the two additional conditions */
            if (is_prime(p + 12) && is_prime(12 * p + 1))
            {
                ++count;
                if (count == n)
                {
                    free(segment);
                    return (seq_t)p;
                }
            }
        }

        free(segment);
        low = high + 1;
    }

    /* if the sought term is beyond MAX_VALUE return 0 (should never happen for n≤10000) */
    return (seq_t)0;
}
