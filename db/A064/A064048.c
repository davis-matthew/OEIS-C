/*
 * A064048  Number of most frequently occurring numbers in the 1-to-n
 *          multiplication table.
 * https://oeis.org/A064048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9410, u16 n <= 9410, u32 n <= 9410, u64 n <= 9410,
 *            u128 n <= 9410, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts frequencies of all products up to n*n and
   returns how many distinct products achieve the maximum
   frequency in the n×n multiplication table. */
#include <stdlib.h>
#include <stdint.h>

/* External types provided by the build system:
   seq_t : unsigned integer type (8/16/32/64 bits or arbitrary precision)
   idx_t : unsigned 64‑bit index type (uint64_t) */

/* Computes a(n) as described in OEIS A064048. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, but keep safe if called with 0 */
        return 0;

    /* Allocate an array of counts for all possible products i*j (1 ≤ i,j ≤ n). */
    /* The maximum product is n*n, which fits in size_t for n ≤ 10000. */
    size_t prod_limit = (size_t)n * (size_t)n;
    uint16_t *cnt = (uint16_t *)calloc(prod_limit + 1, sizeof(uint16_t));
    if (!cnt)
        return 0;       /* allocation failure – unlikely for n ≤ 10000 */

    uint16_t max_freq   = 0; /* maximum frequency seen so far (fits in uint16_t) */
    seq_t    most_count = 0; /* number of products with frequency == max_freq */

    /* Count all product frequencies. */
    for (idx_t i = 1; i <= n; ++i)
    {
        for (idx_t j = 1; j <= n; ++j)
        {
            size_t prod = (size_t)i * (size_t)j;
            uint16_t cur = ++cnt[prod];

            if (cur > max_freq)
            {
                max_freq   = cur;
                most_count = 1;
            }
            else if (cur == max_freq)
            {
                ++most_count;
            }
        }
    }

    free(cnt);
    return most_count;
}
