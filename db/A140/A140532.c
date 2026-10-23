/*
 * A140532  Number of primes with n distinct decimal digits, none of which are
 *          0.
 * https://oeis.org/A140532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A140532:
   a(n) = number of primes containing exactly n distinct decimal digits,
   none of which are 0.  For n ≥ 9 the result is 0 (the largest such
   prime is 98765431).  The function sieves all primes up to 98765431
   once, counts those with a given digit pattern, and then returns the
   pre‑computed count. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* seq_t and idx_t are provided by the calling environment. */
static seq_t counts[9];          /* counts[1]..counts[8] are valid */
static bool computed = false;   /* sieve has been performed */

/* Upper bound – the largest prime having 8 distinct non‑zero digits. */
#define LIMIT 98765431ULL

/* Bit‑set helpers.  Bits are indexed from 0 to LIMIT. */
static inline bool is_composite(const uint8_t *bits, idx_t i)
{
    return (bits[i >> 3] >> (i & 7)) & 1U;
}

static inline void set_composite(uint8_t *bits, idx_t i)
{
    bits[i >> 3] |= (1U << (i & 7));
}

seq_t a(idx_t n)
{
    /* Fast return for out-of‑range indices */
    if (n < 1 || n > 8)
        return 0;

    if (!computed)
    {
        /* Allocate a bitset for numbers up to LIMIT. */
        idx_t bits_len = (LIMIT >> 3) + 1;
        uint8_t *bits = (uint8_t *)malloc(bits_len * sizeof(uint8_t));
        if (!bits) /* allocation failure */
            return 0;
        memset(bits, 0, bits_len);

        /* Sieve of Eratosthenes. */
        for (idx_t i = 2; i * i <= LIMIT; ++i)
        {
            if (is_composite(bits, i))
                continue;
            for (idx_t mult = i * i; mult <= LIMIT; mult += i)
                set_composite(bits, mult);
        }

        /* Count qualifying primes. */
        for (idx_t i = 2; i <= LIMIT; ++i)
        {
            if (is_composite(bits, i))
                continue; /* not a prime */

            idx_t p = i;
            unsigned mask = 0;
            bool repeated = false;
            unsigned distinct = 0;

            while (p > 0)
            {
                unsigned d = p % 10;
                if (d == 0)          /* zero digit – reject */
                {
                    repeated = true;
                    break;
                }
                if ((mask >> d) & 1U)   /* repeated digit – reject */
                {
                    repeated = true;
                    break;
                }
                mask |= 1U << d;
                ++distinct;
                p /= 10;
            }

            if (!repeated && distinct <= 8)
                ++counts[distinct];
        }

        free(bits);
        computed = true;
    }

    return counts[(size_t)n];
}
