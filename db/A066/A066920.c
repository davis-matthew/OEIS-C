/*
 * A066920  Number of partitions of n into lucky parts.
 * https://oeis.org/A066920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = number of partitions of n into lucky parts.
   Lucky numbers are obtained by the standard lucky number sieve:
   start with the odd numbers 1,3,5,7,... ; remove every 3rd number,
   then every 4th remaining number, etc. This implementation
   recomputes the lucky numbers and the partition table whenever a
   larger n is requested. */
#include <stdlib.h>

/* Forward declarations of helper functions */
static void recompute_lucky(idx_t limit);
static seq_t *compute_partitions(idx_t limit);

/* Global static state for memoisation */
static idx_t *lucky = NULL;      /* array of lucky numbers */
static size_t lucky_cnt = 0;     /* number of lucky numbers stored */
static idx_t lucky_last = 0;     /* highest n recomputed so far */

static seq_t *dp = NULL;         /* DP table dp[0..n] */
static idx_t dp_size = 0;        /* allocated size of dp */

/* Helper: recompute lucky numbers up to 'limit' */
static void recompute_lucky(idx_t limit)
{
    /* allocate list of odds up to limit */
    size_t max_odds = (limit + 1) / 2;
    idx_t *list = (idx_t *)malloc(max_odds * sizeof(idx_t));
    size_t len = 0;
    for (idx_t i = 1; i <= limit; i += 2)
        list[len++] = i;

    /* sieve elimination */
    for (size_t idx = 1; idx < len; ++idx) {
        idx_t step = list[idx];
        if (step > len) break;
        size_t new_len = len - len / step;
        idx_t *nlist = (idx_t *)malloc(new_len * sizeof(idx_t));
        size_t pos = 0;
        for (size_t i = 0; i < len; ++i) {
            if ((i + 1) % step != 0)
                nlist[pos++] = list[i];
        }
        free(list);
        list = nlist;
        len = new_len;
    }

    /* free previous lucky array if any */
    if (lucky) free(lucky);
    lucky = (idx_t *)malloc(len * sizeof(idx_t));
    for (size_t i = 0; i < len; ++i)
        lucky[i] = list[i];
    lucky_cnt = len;
    lucky_last = limit;
    free(list);
}

/* Helper: recompute partition DP up to 'limit' */
static seq_t *compute_partitions(idx_t limit)
{
    /* free old dp if it exists */
    if (dp) free(dp);
    dp = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    dp_size = limit + 1;
    if (!dp) return NULL;  /* failed allocation */

    /* initialise */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= limit; ++i)
        dp[i] = (seq_t)0;

    /* generate lucky numbers up to limit */
    if (limit > lucky_last)
        recompute_lucky(limit);

    /* dynamic programming over lucky parts */
    for (size_t j = 0; j < lucky_cnt; ++j) {
        idx_t l = lucky[j];
        if (l > limit) break;
        for (idx_t i = l; i <= limit; ++i)
            dp[i] += dp[i - l];
    }

    return dp;
}

/* Main function required by the build */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n >= dp_size || !dp) {
        /* need to recompute up to n */
        compute_partitions(n);
    }
    return dp[n];
}
