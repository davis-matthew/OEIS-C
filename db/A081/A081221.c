/*
 * A081221  Number of consecutive numbers >= n having at least one square
 *          divisor > 1.
 * https://oeis.org/A081221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of consecutive integers m ≥ n that are not
 *            squarefree (i.e. have a square divisor > 1). The value returned
 *            is the first k such that n + k is squarefree; hence a(n) = (n +
 *            k) – n. Example: a(48) = 3 because 48,49,50 are not squarefree,
 *            while 51 is
 * Range:     u8 n <= 9586, u16 n <= 9586, u32 n <= 9586, u64 n <= 9586,
 *            u128 n <= 9586, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ---- prime table ---- */
#define MAX_PRIMES 70000
static int  primes[MAX_PRIMES];
static int  primes_cnt   = 0;
static uint64_t prime_limit = 0;

/* generate all primes up to `limit` (inclusive) using a simple sieve */
static void generate_primes_upto(uint64_t limit)
{
    if (limit <= prime_limit) return;                /* already known */
    if (limit < 2) return;

    uint64_t size = limit + 1;
    bool *sieve = (bool*)malloc((size_t)size);
    if (!sieve) exit(1);                 /* allocation failure */
    memset(sieve, true, (size_t)size);
    sieve[0] = sieve[1] = false;

    for (uint64_t p = 2; p * p <= limit; ++p)
        if (sieve[p])
            for (uint64_t m = p * p; m <= limit; m += p)
                sieve[m] = false;

    primes_cnt = 0;
    for (uint64_t p = 2; p <= limit; ++p)
        if (sieve[p])
            primes[primes_cnt++] = (int)p;

    free(sieve);
    prime_limit = limit;
}

/* integer square root of a 64‑bit unsigned integer */
static uint64_t isqrt64(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = (mid == 0) ? 0 : mid - 1;
    }
    return high;
}

/* check whether x has a square divisor > 1 (i.e. is NOT squarefree) */
static bool has_square_factor(uint64_t x)
{
    if (x <= 1) return false;                     /* 1 is squarefree */

    uint64_t limit = isqrt64(x);
    if (limit >= prime_limit)     /* ensure prime list covers the search */
        generate_primes_upto(limit);

    for (int i = 0; i < primes_cnt && (uint64_t)primes[i] <= limit; ++i) {
        uint64_t p   = (uint64_t)primes[i];
        uint64_t sq  = p * p;
        if (sq > x) break;
        if (x % sq == 0) return true;
    }
    return false;
}

/* ---- sequence function ---- */
seq_t a(idx_t n)
{
    /* Work with 64‑bit indices; the result fits in seq_t automatically. */
    uint64_t start   = (uint64_t)n;
    uint64_t count   = 0;
    uint64_t m       = start;

    /* Count consecutive integers that are not squarefree */
    while (has_square_factor(m)) {
        ++count;
        ++m;
    }
    return (seq_t)count;                /* cast to the build‑specific seq_t */
}
