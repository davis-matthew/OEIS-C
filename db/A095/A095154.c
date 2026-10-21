/*
 * A095154  Number of 5-block covers of a labeled n-set.
 * https://oeis.org/A095154
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) =
 *            57*a(n-1)-1002*a(n-2)+6562*a(n-3)-15381*a(n-4)+9765*a(n-5)
 *            Initial values (a(3)..a(7)): a(3) = 21 a(4) = 2919 a(5) = 155106
 *            a(6) = 6054006 a(7) = 208493607
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_N 10000   /* maximum n that may be queried */

static seq_t seq[MAX_N + 1];   /* cache of computed values   */
static bool cache_initialized = false;

/* Helper to initialise the cache with the known seed values */
static void init_cache(void)
{
    if (cache_initialized)
        return;

    /* Zero all cache entries */
    for (idx_t i = 0; i <= MAX_N; ++i)
        seq[i] = (seq_t)0;

    /* Set the seed values */
    seq[3] = (seq_t)21;
    seq[4] = (seq_t)2919;
    seq[5] = (seq_t)155106;
    seq[6] = (seq_t)6054006;
    seq[7] = (seq_t)208493607;

    cache_initialized = true;
}

seq_t a(idx_t n)
{
    /* a(n) is defined only for n >= 3 according to OEIS offset */
    if (n < 3)
        return (seq_t)0;

    if (n > MAX_N)
    {
        /* Out of range – user may extend MAX_N if required */
        return (seq_t)0;
    }

    init_cache();                       /* Ensure seed values are present */

    /* Compute any missing terms up to n */
    for (idx_t i = 8; i <= n; ++i)
    {
        if (seq[i] != (seq_t)0)
            continue;                  /* already computed */

        /* Recurrence: a(i) = 57*a(i-1)-1002*a(i-2)+6562*a(i-3)-15381*a(i-4)+9765*a(i-5) */
        seq_t term = (seq_t)57 * seq[i - 1]
                    - (seq_t)1002 * seq[i - 2]
                    + (seq_t)6562 * seq[i - 3]
                    - (seq_t)15381 * seq[i - 4]
                    + (seq_t)9765 * seq[i - 5];
        seq[i] = term;
    }

    return seq[n];
}
