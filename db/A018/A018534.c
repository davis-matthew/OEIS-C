/*
 * A018534  Divisors of 588.
 * https://oeis.org/A018534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 588 in ascending order:
   a(1) = 1, a(18) = 588. The function returns the nth divisor
   of 588 for any n ≥ 1. If n exceeds the number of divisors,
   0 is returned. */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static storage for the sorted divisors of 588 */
    static seq_t *divs = NULL;
    static size_t divc = 0;

    if (!divs) {
        const uint64_t base = 588ULL;
        size_t small_cap = 16, large_cap = 16;
        size_t small_cnt = 0, large_cnt = 0;

        seq_t *small = (seq_t *)malloc(small_cap * sizeof(seq_t));
        seq_t *large = (seq_t *)malloc(large_cap * sizeof(seq_t));

        for (uint64_t i = 1; i * i <= base; ++i) {
            if (base % i == 0) {
                /* add i to the small (ascending) list */
                if (small_cnt >= small_cap) {
                    small_cap *= 2;
                    small = (seq_t *)realloc(small, small_cap * sizeof(seq_t));
                }
                small[small_cnt++] = (seq_t)i;

                uint64_t j = base / i;
                if (j != i) {
                    /* add j to the large (descending) list */
                    if (large_cnt >= large_cap) {
                        large_cap *= 2;
                        large = (seq_t *)realloc(large, large_cap * sizeof(seq_t));
                    }
                    large[large_cnt++] = (seq_t)j;
                }
            }
        }

        /* total number of divisors */
        divc = small_cnt + large_cnt;
        divs = (seq_t *)malloc(divc * sizeof(seq_t));

        /* copy small (already ascending) */
        for (size_t i = 0; i < small_cnt; ++i)
            divs[i] = small[i];

        /* append large in reverse order to obtain ascending order */
        for (size_t i = 0; i < large_cnt; ++i)
            divs[small_cnt + i] = large[large_cnt - 1 - i];

        free(small);
        free(large);
    }

    if (n == 0 || n > divc)
        return (seq_t)0;
    return divs[n - 1];
}
