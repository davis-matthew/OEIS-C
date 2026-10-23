/*
 * A027194  Number of partitions of n into an even number of parts, the least
 *          being 2; also, a(n+2) = number of partitions of n into an odd
 *          number of parts, each >=2.
 * https://oeis.org/A027194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n-2 into an odd number of parts,
 *            each >= 2 (equivalently, number of partitions of n into an even
 *            number of parts, the least being 2). Offset: 1 Computed via
 *            dynamic programming on partitions with parts >= 2
 * Range:     u8 n <= 28, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* static state for memoisation */
static seq_t *dp_even = NULL;          /* partitions into an even number of parts */
static seq_t *dp_odd  = NULL;          /* partitions into an odd  number of parts */
static idx_t  dp_size = 0;             /* current allocated size of dp arrays */
static idx_t  max_computed = 0;        /* largest target for which dp was computed */

/* recompute dp arrays up to target (inclusive) */
static void compute_dp(idx_t target)
{
    /* realloc if needed */
    if (target + 1 > dp_size) {
        free(dp_even);
        free(dp_odd);
        dp_even = (seq_t *)malloc((target + 1) * sizeof(seq_t));
        dp_odd  = (seq_t *)malloc((target + 1) * sizeof(seq_t));
        dp_size = target + 1;
        max_computed = 0;   /* will be recomputed below */
    }

    /* initialise to zero */
    for (idx_t i = 0; i <= target; ++i) {
        dp_even[i] = 0;
        dp_odd[i]  = 0;
    }
    dp_even[0] = 1;  /* empty partition has zero parts (even) */

    /* dynamic programming over part sizes starting at 2 */
    for (idx_t part = 2; part <= target; ++part) {
        for (idx_t sum = part; sum <= target; ++sum) {
            seq_t old_even = dp_even[sum - part];
            seq_t old_odd  = dp_odd[sum - part];
            dp_odd[sum]  += old_even;   /* adding a part flips parity to odd */
            dp_even[sum] += old_odd;    /* adding a part flips parity to even */
        }
    }

    max_computed = target;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* handle offset and small n */
    if (n < 4) {
        return 0;   /* a(1)=a(2)=a(3)=0 by definition */
    }

    /* we need partitions of n-2 */
    idx_t target = n - 2;

    if (target > max_computed) {
        compute_dp(target);
    }

    /* odd number of parts for n-2 gives a(n) */
    return dp_odd[target];
}
