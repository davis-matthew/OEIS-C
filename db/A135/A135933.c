/*
 * A135933  Primes whose integer square root remainder is not a prime.
 * https://oeis.org/A135933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that p - floor(sqrt(p))^2 is not a
 *            prime
 * Range:     u8 n <= 33, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a small cache of primes and sequence values. */

/* --------------------------------------------------------------------- */
/* required includes ---------------------------------------------------- */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>          /* for size_t */
#include "../utils/_floor_isqrt_u64.h"

/* --------------------------------------------------------------------- */
/* internal cache limits ------------------------------------------------ */
#define MAX_PRIMES   200000   /* sufficient for n up to about 10 000 */
#define MAX_SEQ     200000

/* --------------------------------------------------------------------- */
/* internal state ------------------------------------------------------- */
static seq_t seq[MAX_SEQ];
static size_t seq_len = 0;

static uint64_t primes[MAX_PRIMES];
static size_t prime_len = 0;

static uint64_t last_checked = 1;   /* last integer examined for primeness */

/* --------------------------------------------------------------------- */
/* primality test for 64‑bit candidates --------------------------------- */
static bool isPrimeUint(uint64_t n)
{
    if (n < 2) return false;
    uint64_t r = _floor_isqrt_u64((idx_t)n);
    for (size_t i = 0; i < prime_len && primes[i] <= r; ++i)
        if (n % primes[i] == 0) return false;
    return true;
}

/* --------------------------------------------------------------------- */
/* public sequence function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* out of bounds handling */

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n)
    {
        ++last_checked;

        if (isPrimeUint(last_checked))
        {
            /* store newly found prime for future sieving */
            if (prime_len < MAX_PRIMES)
                primes[prime_len++] = last_checked;

            /* compute the remainder r = p - floor(sqrt(p))^2 */
            uint64_t k = _floor_isqrt_u64((idx_t)last_checked);
            uint64_t r = last_checked - k * k;

            /* include p if the remainder is not prime */
            if (!isPrimeUint(r))
            {
                if (seq_len < MAX_SEQ)
                    seq[seq_len++] = (seq_t)last_checked;
            }
        }
    }

    return seq[n - 1];
}
