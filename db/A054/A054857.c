/*
 * A054857  Number of ways to tile a 5 X n region with square tiles of size up
 *          to 5 X 5.
 * https://oeis.org/A054857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+7*a(n-2)+6*a(n-3)
 *            -a(n-4)-5*a(n-5)-2*a(n-6)-3*a(n-7)-a(n-8), with a(0)=1, a(1)=1,
 *            a(2)=8, a(3)=28, a(4)=117, a(5)=472, a(6)=1916, a(7)=7765
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *memo = NULL;          /* array of computed values         */
static size_t memo_cap = 0;         /* allocated capacity               */
static size_t memo_len = 0;         /* number of initialized entries    */

/* Initialise the memoisation array with the first 8 terms. */
static void init_memo(void)
{
    if (memo != NULL)
        return;

    memo_cap = 8;
    memo = alloc_seq_array(memo_cap);
    memo_len = 8;
    memo[0] = (seq_t)1;
    memo[1] = (seq_t)1;
    memo[2] = (seq_t)8;
    memo[3] = (seq_t)28;
    memo[4] = (seq_t)117;
    memo[5] = (seq_t)472;
    memo[6] = (seq_t)1916;
    memo[7] = (seq_t)7765;
}

/* Grow the memo array until it can hold at least 'needed' entries. */
static void ensure_capacity(size_t needed)
{
    if (needed <= memo_cap)
        return;

    size_t new_cap = memo_cap * 2;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *new_arr = alloc_seq_array(new_cap);
    for (size_t i = 0; i < memo_cap; ++i)
        new_arr[i] = memo[i];

    free_seq_array(memo);
    memo = new_arr;
    memo_cap = new_arr ? new_cap : 0;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    init_memo();

    if ((size_t)n < memo_len)
        return memo[(size_t)n];

    /* Ensure the array has enough capacity and compute up to n. */
    ensure_capacity((size_t)n + 1);

    for (idx_t i = (idx_t)memo_len; i <= n; ++i) {
        seq_t t1 = memo[(size_t)(i - 1)];
        seq_t t2 = memo[(size_t)(i - 2)];
        seq_t t3 = memo[(size_t)(i - 3)];
        seq_t t4 = memo[(size_t)(i - 4)];
        seq_t t5 = memo[(size_t)(i - 5)];
        seq_t t6 = memo[(size_t)(i - 6)];
        seq_t t7 = memo[(size_t)(i - 7)];
        seq_t t8 = memo[(size_t)(i - 8)];

        memo[(size_t)i] = 2 * t1 + 7 * t2 + 6 * t3
                        - t4 - 5 * t5 - 2 * t6 - 3 * t7 - t8;
    }

    memo_len = (size_t)n + 1;
    return memo[(size_t)n];
}
