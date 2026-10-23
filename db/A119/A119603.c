/*
 * A119603  Merging prime factors of n-th composite number.
 * https://oeis.org/A119603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of prime factors of the n-th composite
 *            number
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

static uint64_t *composites = NULL;   /* cached composites */
static idx_t composites_cap   = 0;     /* allocated capacity */
static idx_t composites_count = 0;     /* number of cached composites */
static uint64_t next_check    = 3;     /* next integer to test for primality */

/* simple primality test for numbers up to about 10^9 (n <= 10^5 suffices) */
static bool is_prime_simple(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* ensure that composites[0 .. n-1] are filled */
static void ensure_composites(idx_t n)
{
    if (composites_count >= n) return;

    if (!composites)
    {
        composites_cap = 64;
        composites = (uint64_t *)malloc(composites_cap * sizeof(uint64_t));
    }

    while (composites_count < n)
    {
        ++next_check;
        if (!is_prime_simple(next_check))
        {
            /* grow buffer if needed */
            if (composites_count == composites_cap)
            {
                composites_cap <<= 1;  /* double capacity */
                composites = (uint64_t *)realloc(composites,
                                                 composites_cap * sizeof(uint64_t));
            }
            composites[composites_count++] = next_check;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out of range, but harmless */

    ensure_composites(n);
    uint64_t comp = composites[n - 1];

    seq_t result = 0;
    uint64_t rem   = comp;

    for (uint64_t p = 2; p * p <= rem; ++p)
    {
        if (rem % p == 0)
        {
            uint64_t prime = p;
            int exp = 0;
            while (rem % prime == 0)
            {
                rem /= prime;
                ++exp;
            }

            seq_t sp = (seq_t)prime;
            int d = digit_len(sp);
            seq_t pow10d = pow10((idx_t)d);

            for (int i = 0; i < exp; ++i)
                result = result * pow10d + sp;
        }
    }

    if (rem > 1)   /* remaining prime factor */
    {
        seq_t sp = (seq_t)rem;
        int d = digit_len(sp);
        seq_t pow10d = pow10((idx_t)d);
        result = result * pow10d + sp;
    }

    return result;
}
