/*
 * A097427  Integer part of the area of circles with prime radii.
 * https://oeis.org/A097427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(Pi * prime(n)^2)
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static const unsigned long long PI_FIXED = 3141592653589793ULL; /* Pi * 10^15 */
static const unsigned long long SCALE    = 1000000000000000ULL; /* 10^15 */

static uint64_t *primes      = NULL;
static size_t   primes_count = 0;
static int      sieve_done   = 0;

/* Simple sieve that precomputes primes up to a fixed limit.
   The limit is chosen large enough for n <= 10000 (and beyond). */
static void init_primes(void)
{
    if (sieve_done) return;
    const size_t limit = 3000000;          /* 3 million => ~216k primes */
    uint8_t *is_comp = (uint8_t*)calloc(limit + 1, 1);
    if (!is_comp) return;                  /* allocation failed */

    is_comp[0] = is_comp[1] = 1;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                is_comp[j] = 1;
            }
        }
    }
    /* Count primes to allocate array */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (!is_comp[i]) ++cnt;
    primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!primes) { free(is_comp); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[idx++] = i;
        }
    }
    primes_count = idx;
    free(is_comp);
    sieve_done = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard against 0 */
    init_primes();
    if ((size_t)n > primes_count) return 0; /* out of range */

    uint64_t p = primes[n - 1];
    /* Compute floor((PI_FIXED * p^2) / SCALE) using 128‑bit arithmetic for safety */
    unsigned __int128 pp = (unsigned __int128)p * (unsigned __int128)p;
    unsigned __int128 product = (unsigned __int128)PI_FIXED * pp;
    unsigned long long result = (unsigned long long)(product / SCALE);
    return (seq_t)result;
}
