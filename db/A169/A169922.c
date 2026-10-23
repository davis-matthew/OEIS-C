/*
 * A169922  Values of n >= 0 such that 3*n-36+360/(n+10) is an integer.
 * https://oeis.org/A169922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sequence of n ≥ 0 such that 360 mod (n+10) == 0
   (OEIS A169922).  The sequence consists of all numbers d‑10
   where d is a divisor of 360 and d ≥ 10. */
#include <stdlib.h>

static seq_t *cache = NULL;   /* cached list of terms */
static idx_t cache_size = 0;

/* build the cached list on first use */
static void build_cache(void)
{
    if (cache != NULL)
        return;

    /* count relevant divisors of 360 */
    idx_t limit   = 360;
    idx_t count   = 0;
    for (idx_t d = 10; d <= limit; ++d)
        if (limit % d == 0)
            ++count;

    cache = (seq_t *)malloc(count * sizeof(seq_t));  /* cast needed for C++ */
    cache_size = count;

    idx_t pos = 0;
    for (idx_t d = 10; d <= limit; ++d)
        if (limit % d == 0)
            cache[pos++] = (seq_t)(d - 10);
}

/* public interface */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, so n≥1 only */
        return 0;

    build_cache();

    if (n - 1 < cache_size)
        return cache[n - 1];
    else
        return 0;   /* value not defined for n beyond the sequence length */
}
