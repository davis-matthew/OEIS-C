/*
 * A053077  Balanced primes separated from the next lower and next higher
 *          prime neighbors by 42.
 * https://oeis.org/A053077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   uint64_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th prime p such that p‑42 and p+42 are also primes
   (balanced primes with spacing 42).  The function uses a segmented
   sieve with primes up to 1 000 000 and counts balanced triples
   until the desired index is reached. */

/*------------------------------------------------------------*/
/*  Small prime table: all primes ≤ 1e6 (78 498 of them).    */
/*------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/uint64_isqrt.h"

#define SMALL_LIMIT   1000000ULL      /* limit for base prime sieve   */
#define SEG_SIZE      (1 << 17)       /* 131 072 numbers per segment */

static bool small_primes_initialized = false;
static uint32_t small_primes[78498];
static size_t small_prime_count = 0;

/* Generate primes up to SMALL_LIMIT once, store in small_primes array. */
static void init_small_primes(void)
{
    if (small_primes_initialized)
        return;

    uint8_t *marked = (uint8_t *)calloc(SMALL_LIMIT + 1, 1);
    for (uint32_t i = 2; (uint64_t)i * i <= SMALL_LIMIT; ++i)
        if (!marked[i])
            for (uint64_t j = (uint64_t)i * i; j <= SMALL_LIMIT; j += i)
                marked[j] = 1;

    for (uint32_t i = 2; i <= SMALL_LIMIT; ++i)
        if (!marked[i])
            small_primes[small_prime_count++] = i;
    free(marked);
    small_primes_initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* behaviour for n<1 is irrelevant */

    init_small_primes();

    uint64_t target = n;
    uint64_t count  = 0;
    uint64_t lastPrev = 0, last = 0;
    uint64_t low = 2, high;

    uint8_t *isComp = (uint8_t *)calloc(SEG_SIZE, 1);

    while (1) {
        high = low + SEG_SIZE;

        /* clear the segment */
        memset(isComp, 0, SEG_SIZE);

        /* sieve the segment using the small primes up to sqrt(high) */
        uint64_t root = uint64_isqrt(high) + 1;
        for (size_t pi = 0; pi < small_prime_count && small_primes[pi] <= root; ++pi) {
            uint32_t p = small_primes[pi];
            uint64_t start = (low + p - 1) / p * p;
            if (start < (uint64_t)p * 2)
                start = (uint64_t)p * 2;
            for (uint64_t j = start; j < high; j += p)
                isComp[(size_t)(j - low)] = 1;
        }

        /* scan the segment for primes */
        for (size_t i = 0; i < SEG_SIZE; ++i) {
            uint64_t val = low + i;
            if (val < 2)
                continue;
            if (!isComp[i]) {                /* val is prime */
                if (lastPrev && (last - lastPrev == 42) && (val - last == 42)) {
                    ++count;
                    if (count == target)
                        return (seq_t)last;   /* middle prime of the triple */
                }
                lastPrev = last;
                last = val;
            }
        }

        low = high;   /* move to the next segment */
    }
}
