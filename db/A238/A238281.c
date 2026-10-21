/*
 * A238281  a(n) = |{0 < k < n: the two intervals (k*n, (k+1)*n) and ((k+1)*n,
 *          (k+2)*n) contain the same number of primes}|.
 * https://oeis.org/A238281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ 0 < k < n : pi((k+1)n)-pi(k n) = pi((k+2)n)-pi((k+1)n)
 *            }|
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of consecutive intervals of length n that contain the same
   number of primes. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer floor square root for 64‑bit numbers */
static inline idx_t isqrt_u64(idx_t n)
{
    unsigned long long r = 0;
    unsigned long long bit = 1ULL << 62;  /* highest power of four <= 2^64 */
    while (bit > n)
        bit >>= 2;
    while (bit != 0) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return (idx_t)r;
}

/* Prime list lookup */
static uint64_t *prime_list = NULL;      /* array of primes */
static size_t     prime_count = 0;       /* number of primes in prime_list */
static idx_t      current_limit = 0;     /* maximum limit already sieved */

/* Return number of primes <= x */
static inline idx_t pi_idx(idx_t x)
{
    idx_t lo = 0, hi = (idx_t)prime_count;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* Build prime table up to 'limit' (inclusive) if not already present */
static void ensure_primes(idx_t limit)
{
    if (limit <= current_limit)
        return;  /* already enough */

    /* Sieve of Eratosthenes, odd-only */
    idx_t bits  = (limit >> 1) + 1;           /* count of odd numbers <= limit */
    size_t bytes = (bits + 7) >> 3;           /* bytes needed for bitset */
    uint8_t *sieve = (uint8_t *)calloc(bytes, 1);
    if (!sieve)
        return;      /* allocation failed */

    idx_t sqrt_limit = isqrt_u64(limit);
    for (idx_t i = 3; i <= sqrt_limit; i += 2) {
        idx_t ii = i * i;
        if (ii > limit)
            break;
        idx_t step = i << 1;  /* 2 * i, skip even multiples */
        for (idx_t j = ii; j <= limit; j += step) {
            idx_t idx = j >> 1;
            sieve[idx >> 3] |= 1 << (idx & 7);
        }
    }

    /* Count primes to allocate array */
    size_t est_cap = (size_t)(limit / 10);    /* rough estimate */
    uint64_t *tmp = (uint64_t *)malloc(est_cap * sizeof(uint64_t));
    if (!tmp) {
        free(sieve);
        return;
    }
    size_t count = 0;

    /* 2 is prime */
    tmp[count++] = 2;

    for (idx_t n = 3; n <= limit; n += 2) {
        if (n < 3)
            continue;
        idx_t idx = n >> 1;
        if (sieve[idx >> 3] & (1 << (idx & 7)))
            continue;                       /* composite */
        if (count == est_cap) {
            est_cap <<= 1;
            uint64_t *new_tmp = (uint64_t *)realloc(tmp, est_cap * sizeof(uint64_t));
            if (!new_tmp) {
                free(tmp);
                free(sieve);
                return;
            }
            tmp = new_tmp;
        }
        tmp[count++] = n;
    }

    /* Replace global list */
    free(prime_list);
    prime_list = tmp;
    prime_count = count;
    current_limit = limit;

    free(sieve);
}

/* Sequence function: a(n) */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;   /* a(1) = 0 */

    /* Need primes up to (n+1)*n */
    idx_t limit = (n + 1) * n;
    ensure_primes(limit);

    seq_t result = 0;
    for (idx_t k = 1; k < n; ++k) {
        idx_t kn   = k * n;
        idx_t k1n  = (k + 1) * n;
        idx_t k2n  = (k + 2) * n;

        idx_t left  = pi_idx(k1n - 1) - pi_idx(kn);
        idx_t right = pi_idx(k2n - 1) - pi_idx(k1n);
        if (left == right)
            ++result;
    }
    return result;
}
