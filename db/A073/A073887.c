/*
 * A073887  Smallest prime p such that sum of p and the next n-1 primes is a
 *          perfect square, or 1 if no such prime exists.
 * https://oeis.org/A073887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that * p + p_next + … +
 *            p_next_(n-1) is a perfect square, * or 1 if no such prime
 *            exists. * (Offset: 1)
 * Range:     u8 n <= 5, u16 n <= 98, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_perfect_square_uint128, isqrt_128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_perfect_square_uint128.h"
#include "../utils/isqrt_128.h"

/* ---------- Prime sieve (single run) ---------- */
#define PRIME_LIMIT 20000000  /* 20 million is enough for n ≤ 10 000 */

/* Globals needed for memoization */
static bool primes_ready = false;
static size_t primes_cnt  = 0;
static uint32_t *primes   = NULL;

static void
prepare_primes(void)
{
    if (primes_ready) return;

    size_t limit = PRIME_LIMIT + 1;         /* include the last index */
    uint8_t *sieve = (uint8_t*)malloc(limit);
    if (sieve == NULL) return;            /* out of memory → abort initialisation */

    for (size_t i = 0; i < limit; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    /* Simple integer sqrt without using <math.h> */
    size_t sqrt_limit = 1;
    while ((sqrt_limit + 1) * (sqrt_limit + 1) <= limit) ++sqrt_limit;

    for (size_t i = 2; i <= sqrt_limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j < limit; j += i)
                sieve[j] = 0;
        }
    }

    /* Count primes and allocate array */
    for (size_t i = 2; i < limit; ++i) if (sieve[i]) ++primes_cnt;
    primes = (uint32_t*)malloc(primes_cnt * sizeof(uint32_t));
    if (primes == NULL) { free(sieve); return; }

    size_t idx = 0;
    for (size_t i = 2; i < limit; ++i)
        if (sieve[i]) primes[idx++] = (uint32_t)i;

    free(sieve);
    primes_ready = true;
}

/* ---------- Main OEIS function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* offset is 1; n<1 is undefined for the sequence */

    prepare_primes();
    if (!primes_ready) return (seq_t)0;  /* sieve failed */

    if (n > primes_cnt) return (seq_t)1; /* not enough primes to form a block of length n */

    size_t max_start = primes_cnt - n;
    unsigned __int128 sum = 0;

    /* first block */
    for (size_t i = 0; i < (size_t)n; ++i)
        sum += (unsigned __int128)primes[i];

    if (is_perfect_square_uint128(sum))
        return (seq_t)primes[0];

    /* slide over all blocks */
    for (size_t k = 1; k <= max_start; ++k) {
        sum += (unsigned __int128)primes[k + n - 1];
        sum -= (unsigned __int128)primes[k - 1];
        if (is_perfect_square_uint128(sum))
            return (seq_t)primes[k];
    }

    /* no suitable prime found within the sieve range */
    return (seq_t)1;
}
