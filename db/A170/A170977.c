/*
 * A170977  Expansion of (Product_{i=1..m-1} (1 + x^(4*i-1))) * (1 +
 *          x^(2*m-1)) for m = 5.
 * https://oeis.org/A170977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of subsets of {3,7,9,11,15} whose sum equals n
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* m = 5   →   (1 + x^3)(1 + x^7)(1 + x^9)(1 + x^11)(1 + x^15) */

#include <stdbool.h>   /* for bool */
#include <stddef.h>    /* for size_t */

static seq_t cache[46];
static bool initialized = false;

static void init_cache(void)
{
    /* zero the array */
    for (size_t i = 0; i < 46; ++i) cache[i] = 0;

    /* all 2^5 subsets of {3,7,9,11,15} */
    for (idx_t mask = 0; mask < (1ULL << 5); ++mask)
    {
        seq_t sum = 0;
        if (mask & 1ULL) sum += (seq_t)3;
        if (mask & (1ULL << 1)) sum += (seq_t)7;
        if (mask & (1ULL << 2)) sum += (seq_t)9;
        if (mask & (1ULL << 3)) sum += (seq_t)11;
        if (mask & (1ULL << 4)) sum += (seq_t)15;
        cache[(size_t)sum] = cache[(size_t)sum] + 1;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_cache();
    if (n < 46) return cache[(size_t)n];
    return 0;
}
