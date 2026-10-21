/*
 * A089166  Number of primes between squares of successive odd numbers.
 * https://oeis.org/A089166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes between (2n-1)^2 and (2n+1)^2
 *            (exclusive)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*------------------------------------------------------------*/
/*  Integer square‑root for uint64_t (no double, no math.h)   */
/*------------------------------------------------------------*/
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;      /* sqrt(2^64-1) < 2^32 */
    while (lo < hi)
    {
        uint64_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/*------------------------------------------------------------*/
/*  Sieve of Eratosthenes up to a growing limit, stored once  */
/*------------------------------------------------------------*/
static int *global_primes = NULL;
static size_t prime_cnt = 0, prime_cap = 0;
static uint64_t prime_max = 0;

static void ensure_primes(uint64_t limit)
{
    if (limit <= prime_max)
        return;

    size_t sz = limit + 1;
    bool *composite = (bool *)calloc(sz, sizeof(bool));
    if (!composite)
        return;   /* allocation failure – should not happen for our limits */

    for (size_t i = 2; i * i <= limit; ++i)
        if (!composite[i])
            for (size_t j = i * i; j <= limit; j += i)
                composite[j] = true;

    for (size_t i = 2; i <= limit; ++i)
        if (!composite[i])
        {
            if (prime_cnt == prime_cap)
            {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                int *tmp = (int *)realloc(global_primes, prime_cap * sizeof(int));
                if (!tmp) { free(composite); return; }
                global_primes = tmp;
            }
            global_primes[prime_cnt++] = (int)i;
        }

    free(composite);
    prime_max = limit;
}

/*------------------------------------------------------------*/
/*  Main sequence function                                    */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined before the offset, but safety */

    /* bounds: low = (2n-1)^2, high = (2n+1)^2 */
    uint64_t two_n   = 2u * (uint64_t)n;
    uint64_t low     = (two_n - 1) * (two_n - 1);
    uint64_t high    = (two_n + 1) * (two_n + 1);
    uint64_t start   = low + 1;          /* exclusive low */
    uint64_t end     = high - 1;         /* exclusive high */
    if (start > end)
        return 0;

    uint64_t len = end - start + 1;      /* number of integers in segment */
    bool *segment = (bool *)calloc(len, sizeof(bool));
    if (!segment)
        return 0;   /* allocation failure – return zero as safe fallback */

    /* Ensure primes up to sqrt(end) for sieving */
    uint64_t sqrt_end = isqrt_u64(end);
    if (sqrt_end < 2)
        sqrt_end = 2;
    ensure_primes(sqrt_end);

    /* Segmented sieve on [start, end] */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        int p = global_primes[i];
        if ((uint64_t)p * (uint64_t)p > end)
            break;

        uint64_t first = (start + p - 1) / p * p;
        if (first < (uint64_t)p * p)
            first = (uint64_t)p * p;

        for (uint64_t x = first; x <= end; x += p)
            segment[x - start] = true;
    }

    /* Count primes */
    seq_t count = 0;
    for (uint64_t i = 0; i < len; ++i)
    {
        uint64_t val = start + i;
        if (val < 2)
            continue;       /* skip 0 and 1 */
        if (!segment[i])
            ++count;
    }

    free(segment);
    return count;
}
