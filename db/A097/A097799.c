/*
 * A097799  Number of partitions of the n-th deficient number into deficient
 *          numbers.
 * https://oeis.org/A097799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 37, u32 n <= 110, u64 n <= 373,
 *            u128 n <= 1351, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A097799 – number of partitions of the n‑th deficient number into deficient numbers
 *
 *  A deficient number is an integer k whose sum of proper divisors is smaller than k.
 *  For a given n (offset 1) we find the n‑th deficient number, then count the
 *  unordered partitions of that number using only deficient numbers as parts.
 *  The routine uses a classical coin‑change dynamic program – iterating the
 *  deficient numbers in ascending order guarantees that partitions are counted
 *  without order.
 */
#include <stdlib.h>

/*  seq_t and idx_t are supplied by the build system (unsigned integer types or
 *  arbitrary‑precision integer class).  No prototypes for these types are
 *  provided here.  The function a(idx_t) has external linkage. */
static seq_t *dp = NULL;          /* partitions count for 0 .. max_deficient */
static idx_t *def_nums = NULL;    /* sorted list of deficient numbers */
static size_t def_cnt = 0;         /* number of deficient numbers stored   */
static idx_t  def_max = 0;        /* largest number considered for dp      */
static int    initialized = 0;    /* true after full pre‑computation       */

/*  Ensure that def_nums contains at least n_needed deficient numbers and
 *  that dp is computed up to the largest deficient number in that list.   */
static void init_seq(idx_t n_needed)
{
    if (initialized && n_needed <= (idx_t)def_cnt)
        return;                  /* already sufficient */

    /* Choose an upper bound that contains at least n_needed deficient numbers.
     * Start with a moderate size and double until enough are found.   */
    idx_t limit = 20000;
    while (1) {
        /* Sum of proper divisors up to 'limit' */
        uint64_t *sum = (uint64_t *)calloc((size_t)(limit + 1), sizeof(uint64_t));
        if (!sum)
            return;
        for (idx_t i = 1; i <= limit / 2; i++) {
            for (idx_t j = i + i; j <= limit; j += i)
                sum[j] += i;
        }

        /* Count how many numbers are deficient */
        size_t cnt = 0;
        for (idx_t i = 1; i <= limit; i++)
            if (sum[i] < (uint64_t)i)
                cnt++;

        free(sum);

        if ((size_t)n_needed <= cnt)
            break;              /* enough numbers */
        limit *= 2;             /* enlarge search space */
    }
    /* Recompute once more with the final limit and build the deficient list */
    uint64_t *sum = (uint64_t *)calloc((size_t)(limit + 1), sizeof(uint64_t));
    for (idx_t i = 1; i <= limit / 2; i++) {
        for (idx_t j = i + i; j <= limit; j += i)
            sum[j] += i;
    }
    def_nums = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!def_nums) {
        free(sum);
        return;
    }
    def_cnt = 0;
    for (idx_t i = 1; i <= limit; i++)
        if (sum[i] < (uint64_t)i)
            def_nums[def_cnt++] = i;
    free(sum);
    def_max = limit;

    /* Allocate dp array and compute partitions using all deficient numbers as coins */
    dp = (seq_t *)calloc((size_t)(limit + 1), sizeof(seq_t));
    if (!dp) {
        free(def_nums);
        def_nums = NULL;
        return;
    }
    dp[0] = (seq_t)1;           /* one way to partition 0 */
    for (size_t k = 0; k < def_cnt; k++) {
        idx_t coin = def_nums[k];
        for (idx_t m = coin; m <= limit; m++)
            dp[m] += dp[m - coin];
    }
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* sequence is offset 1 */
    init_seq(n);
    if (!initialized)
        return (seq_t)0;
    if ((idx_t)n > (idx_t)def_cnt)
        return (seq_t)0;          /* should not happen for supported ranges */
    idx_t target = def_nums[n - 1];    /* n‑th deficient number (1‑based) */
    return dp[target];
}
