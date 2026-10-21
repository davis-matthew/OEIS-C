/*
 * A056930  Average of smallest prime greater than n^2 and largest prime less
 *          than (n+1)^2.
 * https://oeis.org/A056930
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (A007491(n)+A053001(n+1))/2 (offset 2)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A007491(n)  – smallest prime > n^2
   A053001(n)  – largest prime < n^2                 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* -- helpers -------------------------------------------------------------- */

static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t x = 0, b = (uint64_t)1 << 32;
    while (b) {
        uint64_t t = x + b;
        if (t <= n) {
            n -= t;
            x = (x >> 1) + b;
        } else
            x >>= 1;
        b >>= 2;
    }
    return x;
}

/* -- Sieving --------------------------------------------------------------- */

static unsigned char *is_prime = NULL;   /* 1 = prime, 0 = composite */
static uint64_t sieve_limit = 0;         /* highest number that has been
                                            sieved up to (inclusive) */

static void build_sieve(uint64_t limit)
{
    if (limit <= sieve_limit) return;      /* already sufficient */

    unsigned char *new_array = (unsigned char *)malloc(limit + 1);
    if (!new_array) exit(1);               /* allocation failure */

    /* assume all >=2 are prime initially */
    memset(new_array, 1, limit + 1);
    if (limit >= 0) new_array[0] = 0;
    if (limit >= 1) new_array[1] = 0;

    uint64_t sq = isqrt_u64(limit);
    for (uint64_t i = 2; i <= sq; ++i)
        if (new_array[i]) {
            uint64_t ii = i * i;
            for (uint64_t j = ii; j <= limit; j += i)
                new_array[j] = 0;
        }

    if (is_prime) free(is_prime);
    is_prime = new_array;
    sieve_limit = limit;
}

/* -- sequence -------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* the definition is only valid for n>=2; for other values return 0
       (the value is irrelevant because the OEIS offset is 2). */
    if (n < 2) return (seq_t)0;

    /* the largest value we ever need to prime‑test */
    uint64_t n_sq  = (uint64_t)n * (uint64_t)n;
    uint64_t n1_sq = (uint64_t)(n + 1) * (uint64_t)(n + 1);

    build_sieve(n1_sq);                     /* primes up to (n+1)^2 */

    /* A007491(n): smallest prime > n^2  */
    uint64_t p_small = n_sq + 1;
    while (p_small <= n1_sq && !is_prime[p_small]) ++p_small;

    /* A053001(n+1): largest prime < (n+1)^2  */
    uint64_t p_large = n1_sq - 1;
    while (p_large >= 2 && !is_prime[p_large]) --p_large;

    /* average, integer division is correct because the sum is even */
    return ((seq_t)p_small + (seq_t)p_large) / (seq_t)2;
}
