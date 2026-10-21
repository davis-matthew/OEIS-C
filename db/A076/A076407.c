/*
 * A076407  Sum of perfect powers <= n.
 * https://oeis.org/A076407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of perfect powers <= n * (1^k, 2^k, 3^k, ...) with k
 *            >= 1 and the arguments <= n
 * Range:     u8 n <= 63, u16 n <= 2808, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed for the implementation. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Utility: floor of log2 for uint64_t values. */
static inline idx_t floor_log2_uint64(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

/* Compute base^exp in seq_t type and stop if the value would exceed limit.
 * If the result exceeds limit, the function returns limit + 1. */
static inline seq_t pow_limit(seq_t base, idx_t exp, seq_t limit)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
    {
        if (result > limit / base) return limit + 1;
        result *= base;
    }
    return result;
}

/* Static buffer used for marking perfect powers.  It is expanded as needed. */
static bool *seen = NULL;          /* pointer to the bool array */
static idx_t seen_cap = 0;         /* current capacity of 'seen' (in elements) */

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* safety for out-of-range indices */

    /* Ensure the buffer has capacity for indices up to n. */
    if (seen_cap < n + 1)
    {
        idx_t new_cap = n + 1;
        bool *tmp = (bool *)realloc(seen, (size_t)new_cap * sizeof(bool));
        if (!tmp) return 0;                     /* allocation failure */
        if (seen) /* only the new part needs to be cleared */
            memset(tmp + seen_cap, 0,
                   (size_t)(new_cap - seen_cap) * sizeof(bool));
        else
            memset(tmp, 0, (size_t)new_cap * sizeof(bool));
        seen = tmp;
        seen_cap = new_cap;
    }
    else
    {
        /* Clear all entries that will be used for this call. */
        memset(seen, 0, (size_t)(n + 1) * sizeof(bool));
    }

    /* 1 is a perfect power (1^k for any k). */
    seen[1] = true;

    /* Find the largest exponent k such that 2^k <= n. */
    idx_t max_k = floor_log2_uint64(n);

    /* Enumerate all perfect powers for exponents k >= 2. */
    for (idx_t k = 2; k <= max_k; ++k)
    {
        for (idx_t b = 2;; ++b)
        {
            seq_t val = pow_limit((seq_t)b, k, (seq_t)n);
            if (val > (seq_t)n) break;
            seen[(idx_t)val] = true;
        }
    }

    /* Sum all numbers marked as perfect powers up to n. */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        if (seen[i]) sum += (seq_t)i;
    }
    return sum;
}
