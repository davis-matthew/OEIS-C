/*
 * A087867  n^2-79*n+1601 as n runs through the lucky numbers.
 * https://oeis.org/A087867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = L_n^2 - 79*L_n + 1601 where L_n is the n-th lucky number
 *            (A000959)
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* static storage for lucky numbers */
static uint64_t *lucky = NULL;      /* array of lucky numbers */
static size_t   lucky_capacity = 0;/* allocated space for lucky */
static size_t   lucky_count    = 0;/* number of lucky numbers stored */

/* generate at least 'need' lucky numbers */
static void generate_lucky_up_to(size_t need)
{
    if (lucky_count >= need) return;

    /* choose an initial bound that is usually sufficient; increase if needed */
    size_t bound = (need > 50 ? need * 20 : 2000);
    for (;;)
    {
        /* build list of odd numbers up to bound            */
        size_t odds = bound / 2 + 1;
        uint64_t *nums = (uint64_t *)malloc(odds * sizeof(uint64_t));
        if (!nums) exit(EXIT_FAILURE);
        for (size_t i = 0, val = 1; i < odds; ++i, val += 2)
            nums[i] = val;
        size_t nums_len = odds;

        /* classic sieve: iterate over lucky numbers as steps   */
        for (size_t i = 1; i < nums_len; ++i)
        {
            uint64_t step = nums[i];
            if ((size_t)step > nums_len) break;          /* no more removals */

            size_t new_len = 0;
            for (size_t j = 0; j < nums_len; ++j)
            {
                if ((j + 1) % step != 0)
                    nums[new_len++] = nums[j];
            }
            nums_len = new_len;
        }

        /* keep the lucky numbers in static storage */
        if (lucky_capacity < nums_len)
        {
            uint64_t *tmp = (uint64_t *)realloc(lucky,
                                                nums_len * sizeof(uint64_t));
            if (!tmp) { free(nums); exit(EXIT_FAILURE); }
            lucky = tmp;
            lucky_capacity = nums_len;
        }
        memcpy(lucky, nums, nums_len * sizeof(uint64_t));
        lucky_count = nums_len;
        free(nums);

        if (lucky_count >= need) break;
        bound *= 2; /* try a larger bound and redo */
    }
}

/* required external function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset is 1, n>=1 by definition */

    generate_lucky_up_to((size_t)n);

    seq_t L = (seq_t)lucky[(size_t)(n - 1)];
    return L * L - (seq_t)79 * L + (seq_t)1601;
}
