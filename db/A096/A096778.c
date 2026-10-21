/*
 * A096778  Number of partitions of n with at most two even parts.
 * https://oeis.org/A096778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Number of partitions of n with at most two even parts
 *            (OEIS A096778). The sequence is computed by dynamic programming
 *            over the parts 1,2,…,n. For each part p we keep three tables:
 *            dp0[n] – partitions of n with 0 even parts, dp1[n] – partitions
 *            of n with 1 even part, dp2[n] – partitions of...
 * Range:     u8 n <= 18, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp0 = NULL, *dp1 = NULL, *dp2 = NULL;
static idx_t  capacity = 0; /* current number of allocated entries */

/* Rebuild the tables up to the largest index N needed. */
static void recompute(idx_t N)
{
    size_t new_cap = (size_t)(N + 1);             /* indices 0..N inclusive */
    if (capacity < new_cap) {
        dp0 = (seq_t *)realloc(dp0, new_cap * sizeof(seq_t));
        dp1 = (seq_t *)realloc(dp1, new_cap * sizeof(seq_t));
        dp2 = (seq_t *)realloc(dp2, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }
    /* initialise all entries to zero */
    for (idx_t i = 0; i < capacity; ++i) {
        dp0[i] = (seq_t)0;
        dp1[i] = (seq_t)0;
        dp2[i] = (seq_t)0;
    }
    dp0[0] = (seq_t)1;   /* one way to partition 0 with no even parts */

    for (idx_t part = 1; part <= N; ++part) {
        if ((part & 1U) == 0) {   /* even part */
            for (idx_t n = part; n <= N; ++n) {
                dp2[n] += dp1[n - part];
                dp1[n] += dp0[n - part];
            }
        } else {                  /* odd part */
            for (idx_t n = part; n <= N; ++n) {
                dp2[n] += dp2[n - part];
                dp1[n] += dp1[n - part];
                dp0[n] += dp0[n - part];
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n >= capacity)
        recompute(n);
    return dp0[n] + dp1[n] + dp2[n];
}
