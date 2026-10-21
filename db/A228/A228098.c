/*
 * A228098  Number of primes p > prime(n) and such that prime(n)*p <
 *          prime(n+1)^2.
 * https://oeis.org/A228098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A228098:
   a(n) = # { p : p > prime(n) and prime(n)*p < prime(n+1)^2 } */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* -------------------------------------------------------------------- */
/*  Prime generation (sieve)                                            */
/* -------------------------------------------------------------------- */
#define _LIMIT_PRIME 200000U          /* big enough for n up to ~10000 */

static uint8_t _sieve[_LIMIT_PRIME + 1];
static uint32_t _prime_list[20000];   /* π(200000) ≈ 17984 */
static size_t _prime_cnt = 0;
static bool   _sieved   = false;

static void _init_sieve(void)
{
    if (_sieved) return;

    /* Initially consider all numbers prime (0 = prime, 1 = composite) */
    for (size_t i = 0; i <= _LIMIT_PRIME; ++i) _sieve[i] = 0;

    for (size_t i = 2; i <= _LIMIT_PRIME; ++i) {
        if (_sieve[i] == 0) {                          /* i is prime   */
            _prime_list[_prime_cnt++] = (uint32_t)i;
            if ((uint64_t)i * i <= _LIMIT_PRIME) {
                for (size_t j = i * i; j <= _LIMIT_PRIME; j += i)
                    _sieve[j] = 1;                     /* composite   */
            }
        }
    }
    _sieved = true;
}

/* -------------------------------------------------------------------- */
/*  Utility: count primes <= limit that start after index 'start'      */
/* -------------------------------------------------------------------- */
static size_t _count_primes_leq(uint64_t limit, size_t start_idx)
{
    size_t lo = 0, hi = _prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if ((uint64_t)_prime_list[mid] <= limit)
            lo = mid + 1;       /* first > limit will be at lo */
        else
            hi = mid;
    }
    /* lo is the first index with prime > limit */
    return (lo > start_idx) ? lo - start_idx : (size_t)0;
}

/* -------------------------------------------------------------------- */
/*  Main sequence function                                               */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1U) return 0;     /* undefined, but safe */

    _init_sieve();

    /* Indices of prime(n) and prime(n+1) in the 0‑based _prime_list */
    size_t idx_q = (size_t)n - 1U;   /* prime(n)   */
    size_t idx_r = (size_t)n;       /* prime(n+1) */

    if (idx_r >= _prime_cnt) return 0;   /* safety for out‑of‑range n */

    uint64_t q = (uint64_t)_prime_list[idx_q];
    uint64_t r = (uint64_t)_prime_list[idx_r];

    /* Max p satisfying q * p < r^2  ->  p <= ((r^2)-1)/q   */
    uint64_t limit = ((uint64_t)r * r - 1U) / q;

    /* Count primes strictly greater than q and <= limit */
    size_t count = _count_primes_leq(limit, idx_q + 1);

    return (seq_t)count;
}
