/*
 * A018554  Divisors of 620.
 * https://oeis.org/A018554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n‑th smallest divisor of 620 (offset 1)
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Static array holding the divisors of 620 in ascending order. */
static seq_t divisors[12];
static int initialized = 0;

/* Compute all divisors of 620 once. */
static void init_divisors(void)
{
    if (initialized)
        return;

    const uint64_t TARGET = 620ULL;
    size_t count = 0;
    for (uint64_t d = 1; d <= TARGET; ++d)
    {
        if (TARGET % d == 0)
        {
            divisors[count++] = (seq_t)d;
        }
    }
    /* For 620 = 2^2 * 5 * 31 we expect exactly 12 divisors. */
    (void)count; /* silence unused variable warning if build doesn't use it */
    initialized = 1;
}

seq_t a(idx_t n)
{
    init_divisors();

    /* Ensure we are within the finite range. */
    if (n < 1 || n > 12)
        return (seq_t)0;

    /* Convert to zero‑based index safely. */
    return divisors[(size_t)(n - 1)];
}
