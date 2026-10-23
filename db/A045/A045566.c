/*
 * A045566  Numbers whose factorial has '76' as its final two digits before
 *          the trailing zeros.
 * https://oeis.org/A045566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers whose factorial has '76' as its final two non‑zero digits before the trailing zeros.
    The implementation keeps a rolling product of n! with all 2 and 5 factors removed,
    tracks the excess 2s, and tests whether the last non‑zero two digits are 76. 
    The computation does not rely on any hard‑coded terms and supports all provided
    integer types for seq_t and idx_t. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute 2^exp mod 100 using binary exponentiation. */
static inline seq_t pow2_mod100(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 2;
    while (exp)
    {
        if (exp & 1ULL) result = (result * base) % 100;
        base = (base * base) % 100;
        exp >>= 1;
    }
    return result;
}

/* Maximum number of terms to cache; sufficient for n up to about 10000. */
#define MAX_TERMS 500000

seq_t a(idx_t n)
{
    /* Incrementally generate the sequence until the nth element is found. */
    static seq_t     vals[MAX_TERMS];
    static size_t    cnt   = 0;            /* number of terms stored */
    static idx_t     cur   = 0;            /* current integer being examined */
    static seq_t     prod  = 1;            /* product of numbers with 2s and 5s removed, mod 100 */
    static idx_t     twos  = 0;            /* total count of factor 2 removed */
    static idx_t     fives = 0;            /* total count of factor 5 removed */

    while (cnt < n)
    {
        ++cur;             /* next integer to examine */

        /* Remove all factors of 2 from cur, increasing twos counter. */
        idx_t x = cur;
        while ((x & 1ULL) == 0ULL)
        {
            x >>= 1;
            ++twos;
        }
        /* Remove all factors of 5 from cur, increasing fives counter. */
        while (x % 5ULL == 0ULL)
        {
            x /= 5ULL;
            ++fives;
        }

        /* Update the product modulo 100. */
        prod = (prod * (x % 100ULL)) % 100ULL;

        /* Compute the last two non‑zero digits: prod * 2^(twos‑fives) mod 100. */
        idx_t   extra = twos - fives;                /* surplus factor 2s */
        seq_t   last  = (prod * pow2_mod100(extra)) % 100ULL;

        /* If the condition is satisfied, store the value. */
        if (last == 76)
        {
            if (cnt >= MAX_TERMS)
            {
                /* In practice this should never happen for n up to 10 000. */
                exit(1);
            }
            vals[cnt++] = (seq_t)cur;
        }
    }

    return vals[n - 1];
}
