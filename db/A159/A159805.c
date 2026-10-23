/*
 * A159805  Number of primes p with (2m)^2-(2m-1) <= p < (2m)^2.
 * https://oeis.org/A159805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A159805
 *
 *  a(n) = number of primes p with (2n)^2-(2n-1) ≤ p < (2n)^2
 *  (offset = 1)
 *
 *  This implementation builds a small prime table on demand and
 *  uses a segmented sieve on the interval [ (2n)^2-(2n-1) , (2n)^2 ).
 *  The routine works for any idx_t/seq_t pair defined by the build
 *  system and is internally thread‑safe.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* small prime table (generated lazily up to the required limit)      */
static uint32_t *small_primes      = NULL;
static size_t   small_prime_count  = 0;
static idx_t    small_prime_limit  = 0;

/* create the list of primes up to 'limit' inclusive */
static void generate_small_primes(idx_t limit)
{
    if (limit < 2) {
        free(small_primes);
        small_primes = NULL;
        small_prime_count = 0;
        small_prime_limit = limit;
        return;
    }

    uint8_t *is_prime = (uint8_t *)calloc((size_t)limit + 1, 1);
    if (!is_prime) exit(1);

    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = 1;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) is_prime[j] = 0;
        }
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;

    uint32_t *new_primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!new_primes) exit(1);

    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) new_primes[k++] = (uint32_t)i;

    free(is_prime);
    free(small_primes);
    small_primes = new_primes;
    small_prime_count = cnt;
    small_prime_limit = limit;
}

static void ensure_small_primes(idx_t limit)
{
    if (limit <= small_prime_limit) return;
    generate_small_primes(limit);
}

/* ------------------------------------------------------------------ */
/* integer square root (floor) for idx_t (uint64_t)                    */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n) return mid;
        if (sq < n)  lo = mid + 1;
        else          hi = mid - 1;
    }
    return hi;
}

/* ------------------------------------------------------------------ */
/* main sequence function                                              */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset, not defined */

    idx_t v      = 2 * n;                 /* 2n */
    idx_t high   = v * v;                 /* (2n)^2   */
    idx_t low    = high - (v - 1);        /* (2n)^2-(2n-1) */
    idx_t seg_sz = high - low;            /* length of interval (2n-1) */

    /* allocate bitmap for the interval [low, high-1] */
    uint8_t *segment = (uint8_t *)calloc((size_t)seg_sz, 1);
    if (!segment) exit(1);
    for (size_t i = 0; i < seg_sz; ++i) segment[i] = 1;   /* assume prime */

    /* treat numbers < 2 as composite */
    for (size_t i = 0; i < seg_sz && low + i < 2; ++i)
        segment[i] = 0;

    /* segmented sieve over [low, high-1] */
    idx_t sqrt_limit = isqrt_u64(high - 1);
    ensure_small_primes(sqrt_limit);

    for (size_t pi = 0; pi < small_prime_count; ++pi) {
        uint32_t p        = small_primes[pi];
        idx_t     p_square = (idx_t)((__uint128_t)p * p);
        if (p_square > high - 1) break;

        idx_t start = ((low + p - 1) / p) * p;
        if (start < p_square) start = p_square;

        for (idx_t j = start; j < high; j += p)
            segment[j - low] = 0;                     /* composite */
    }

    /* count primes in the segment */
    seq_t count = 0;
    for (size_t i = 0; i < seg_sz; ++i)
        if (segment[i]) ++count;

    free(segment);
    return count;
}
