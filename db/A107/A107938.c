/*
 * A107938  Numbers n such that n and n-th prime have only one common digit =
 *          7.
 * https://oeis.org/A107938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer m such that m and the m‑th
 *            prime share exactly one common digit, which is 7
 * Range:     u8 n <= 8, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   digit_mask_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_mask_uint64.h"

/* ---- constants ---- */
#define LIMIT           2000000   /* sieve limit – sufficient for n up to ~10000 */
#define MAX_PRIMES      200000    /* enough to hold all primes ≤ LIMIT */

/* ---- static storage ---- */
static bool             initialized = false;
static unsigned char    is_comp[LIMIT + 1] = {0};   /* sieve: 0 = prime candidate, 1 = composite */
static uint32_t         primes[MAX_PRIMES];
static size_t           primes_cnt          = 0;

/* ---- initialise sieve and prime list ---- */
static void init_sieve(void)
{
    if (initialized) return;

    /* simple sieve of Eratosthenes */
    for (uint32_t i = 2; i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = 1;

    /* collect primes */
    for (uint32_t i = 2; i <= LIMIT && primes_cnt < MAX_PRIMES; ++i)
        if (!is_comp[i])
            primes[primes_cnt++] = i;

    initialized = true;
}

/* ---- main sequence function ---- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 – graceful exit */

    init_sieve();

    uint64_t candidate = 0;         /* current integer to test */
    size_t   found     = 0;         /* number of terms found so far */

    const uint16_t MASK7 = 1u << 7; /* bit mask for digit '7' */

    while (found < n)
    {
        /* next integer candidate */
        ++candidate;
        if (candidate > primes_cnt) break;   /* safety – sieve limit exceeded */

        uint32_t prime = primes[candidate - 1];  /* candidate-th prime (0‑based) */

        uint16_t mask_cand = digit_mask_uint64(candidate);
        uint16_t mask_prime = digit_mask_uint64(prime);

        /* check that the only common digit is '7' */
        if ((mask_cand & mask_prime & MASK7) != 0 &&
            (mask_cand & mask_prime) == MASK7)
        {
            ++found;
            if (found == (size_t)n)
                return (seq_t)candidate;
        }
    }

    /* in the unlikely event we ran past available primes, return 0 */
    return 0;
}
