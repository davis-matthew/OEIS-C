/*
 * A230476  a(n) = Sum_{i=1..n} d(8*i+1) - Sum_{i=1..n} d(2*i+1), where d(n) =
 *          A000005(n) is the number of divisors of n.
 * https://oeis.org/A230476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} d(8*i+1) - Sum_{i=1..n} d(2*i+1)
 * Range:     u8 n <= 369, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* d(k) is the number of positive divisors of k. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *div_counts = NULL;   /* divisor counts up to the current limit */
static idx_t div_cap = 0;          /* allocated size of div_counts */

/* Ensure divisor counts are available up to 'limit'. Recomputes from scratch
   each time a larger limit is requested. */
static void ensure_div_counts(idx_t limit)
{
    if (div_cap > limit)
        return;                     /* already sufficient */

    /* Allocate new array of size limit+1, initialized to zero. */
    seq_t *new_counts = (seq_t*)calloc((size_t)limit + 1, sizeof(seq_t));
    if (!new_counts)
        exit(1);                    /* out of memory */

    /* Simple sieve: for each i, increment divisor counts of its multiples. */
    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i)
            ++new_counts[j];
    }

    /* Free old array and replace pointers/capacity. */
    free(div_counts);
    div_counts = new_counts;
    div_cap    = limit + 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* defined behavior for n=0 */

    /* Largest number we need to access is 8*n+1. */
    idx_t max_needed = 8 * n + 1;
    ensure_div_counts(max_needed);

    seq_t diff = 0;
    for (idx_t i = 1; i <= n; ++i) {
        diff += div_counts[8 * i + 1];
        diff -= div_counts[2 * i + 1];
    }
    return diff;
}
