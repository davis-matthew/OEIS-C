/*
 * A049667  a(n) = Fibonacci(7*n)/13.
 * https://oeis.org/A049667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(n)=29*a(n-1)+a(n-2) for n>1 */

#include "../utils/ensure_cache.h"

static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;
static idx_t seq_computed = 1; /* highest index already computed */

seq_t a(idx_t n)
{
    /* Handle the base cases directly and initialise cache if needed */
    if (n == 0) return 0;
    if (n == 1) return 1;

    if (seq_cache == NULL) {
        /* Allocate space for at least indices 0 and 1 */
        ensure_cache(1, &seq_cache, &seq_cap);
        seq_cache[0] = 0;
        seq_cache[1] = 1;
    }

    if ((idx_t)seq_cap <= n) {
        /* Ensure capacity for indices up to n */
        ensure_cache(n, &seq_cache, &seq_cap);
    }

    if (n > seq_computed) {
        for (idx_t i = seq_computed + 1; i <= n; ++i) {
            seq_cache[i] = ((seq_t)29 * seq_cache[i - 1]) + seq_cache[i - 2];
        }
        seq_computed = n;
    }

    return seq_cache[n];
}
