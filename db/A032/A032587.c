/*
 * A032587  Lucky numbers ending with digit 5.
 * https://oeis.org/A032587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th lucky number that is a multiple of 5, i.e. the
 *            n-th element of the sequence of lucky numbers congruent to 0 mod
 *            5
 * Range:     u8 n <= 9, u16 n <= 1166, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free, abort */

/* Cached array of numbers that are divisible by 5. */
static seq_t *div5_cache = NULL;  /* array of the lucky numbers %5==0   */
static size_t div5_cnt  = 0;      /* amount of elements stored         */
static idx_t  last_limit = 0;     /* highest upper bound used so far   */

/* Grow the cache to hold at least `need` elements. */
static void ensure_div5_cached(size_t need)
{
    while (div5_cnt < need) {
        /* Expand the numeric limit until we have enough divisible lucky numbers. */
        idx_t new_limit = last_limit ? last_limit << 1 : 1000;
        if (new_limit <= last_limit) new_limit = last_limit + 1000; /* avoid overflow */

        /* Generate all lucky numbers up to new_limit. */
        size_t lucky_len = 0;
        seq_t *lucky = (seq_t *)malloc(((size_t)new_limit + 1) / 2 * sizeof(seq_t));
        if (!lucky) abort();

        /*
         * Start with all odd numbers 1,3,5,... up to new_limit.
         * The index j corresponds to value 2*j+1.
         */
        size_t sz = ((size_t)new_limit + 1) / 2;
        for (size_t j = 0; j < sz; ++j)
            lucky[j] = (seq_t)(2 * j + 1);

        /* Perform the classic lucky number sieve. */
        size_t idx = 1; /* the second element in the list (value 3) */
        while (idx < sz) {
            seq_t step = lucky[idx];               /* step size to remove each time */
            size_t step_usize = (size_t)step;      /* safe conversion: step <= sz          */
            if (step_usize == 0 || step_usize > sz) break;

            size_t new_sz = 0;
            for (size_t j = 0; j < sz; ++j) {
                /* keep elements that are not removed */
                if (((j + 1) % step_usize) != 0)
                    lucky[new_sz++] = lucky[j];
            }
            sz = new_sz;
            ++idx;
        }

        lucky_len = sz; /* final number of lucky numbers up to new_limit */

        /* Count how many of them are divisible by 5. */
        size_t new_div_cnt = 0;
        for (size_t i = 0; i < lucky_len; ++i)
            if ((lucky[i] % (seq_t)5) == 0)
                ++new_div_cnt;

        /* Create a new array holding only the divisible lucky numbers. */
        seq_t *new_div = (seq_t *)malloc(new_div_cnt * sizeof(seq_t));
        if (!new_div) abort();

        size_t pos = 0;
        for (size_t i = 0; i < lucky_len; ++i) {
            if ((lucky[i] % (seq_t)5) == 0)
                new_div[pos++] = lucky[i];
        }

        /* Replace the old cache. */
        free(div5_cache);
        div5_cache = new_div;
        div5_cnt   = new_div_cnt;
        last_limit = new_limit;

        free(lucky);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1 – behaviour for n<1 is irrelevant */

    ensure_div5_cached((size_t)n);
    return div5_cache[(size_t)n - 1];
}
