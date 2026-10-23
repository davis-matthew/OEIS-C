/*
 * A158528  Sum of primes between consecutive positive cubes.
 * https://oeis.org/A158528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ{ p prime | n³ < p < (n+1)³ } p (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 114, u64 n <= 600,
 *            u128 n <= 600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static prime table used for sieving */
static uint32_t *PRIMES = NULL;
static size_t PRIME_CNT = 0;
static bool PRIMES_INIT = false;

/* Build all primes up to 1 001 000 (≈√((10 001)³)). */
static void init_primes(void)
{
    if (PRIMES_INIT)
        return;

    const idx_t LIMIT = 1000100;
    bool *sieve = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!sieve)
        abort();

    sieve[0] = sieve[1] = true;
    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (!sieve[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = true;

    size_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i])
            ++cnt;

    PRIMES = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!PRIMES)
        abort();

    size_t k = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i])
            PRIMES[k++] = (uint32_t)i;

    PRIME_CNT = cnt;
    PRIMES_INIT = true;
    free(sieve);
}

/* Sum of primes in the exclusive interval (low, high) via segmented sieve. */
static seq_t segmented_sum(idx_t low, idx_t high)
{
    const idx_t BLOCK = 1 << 20; /* 1 048 576 numbers per block */
    seq_t sum = 0;

    for (idx_t block_low = low; block_low <= high; block_low += BLOCK)
    {
        idx_t block_high = block_low + BLOCK - 1;
        if (block_high > high)
            block_high = high;

        idx_t sz = block_high - block_low + 1;
        size_t byte_len = (size_t)((sz + 7) >> 3);
        uint8_t *mark = (uint8_t *)calloc(byte_len, 1);
        if (!mark)
            abort();

        /* Mark composites using the precomputed primes */
        for (size_t pi = 0; pi < PRIME_CNT; ++pi)
        {
            uint32_t p = PRIMES[pi];
            uint64_t pp = (uint64_t)p * p;
            if (pp > (uint64_t)block_high)
                break;

            uint64_t start = (block_low + p - 1) / p * p;
            if (start < pp)
                start = pp;

            for (uint64_t j = start; j <= block_high; j += p)
            {
                idx_t idx = (idx_t)(j - block_low);
                mark[idx >> 3] |= (uint8_t)(1u << (idx & 7));
            }
        }

        /* Accumulate surviving primes */
        for (idx_t i = 0; i < sz; ++i)
            if (!(mark[i >> 3] & (uint8_t)(1u << (i & 7))))
            {
                idx_t val = block_low + i;
                if (val >= 2)
                    sum += (seq_t)val;
            }

        free(mark);
    }

    return sum;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1, safe fallback */

    init_primes();

    idx_t low  = (idx_t)n * n * n + 1;
    idx_t n1   = n + 1;
    idx_t high = (idx_t)n1 * n1 * n1 - 1;

    return segmented_sum(low, high);
}
