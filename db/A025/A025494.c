/*
 * A025494  Squares which are the sum of factorials of distinct integers
 *          (probably finite).
 * https://oeis.org/A025494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is the nth square obtainable as the sum of distinct
 *            factorials. (Computed by enumerating all subsets of 0!…15! and
 *            selecting perfect squares.)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static seq_t *squares = NULL;     /* array of found squares          */
static size_t sq_cnt = 0;         /* number of squares stored        */
static size_t sq_cap = 0;         /* current allocation capacity     */
static bool initialized = false;  /* flag to ensure one-time init     */

static void add_square(seq_t val)
{
    if (sq_cap == 0 || sq_cnt == sq_cap) {
        size_t new_cap = (sq_cap == 0) ? 64 : sq_cap << 1;
        seq_t *tmp = (seq_t *)realloc(squares, new_cap * sizeof(seq_t));
        if (!tmp) { free(squares); exit(1); }
        squares = tmp;
        sq_cap = new_cap;
    }
    squares[sq_cnt++] = val;
}

static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

static void compute_factorials(seq_t fact[16])
{
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= 15; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;
}

static void init_sequence(void)
{
    seq_t fact[16];
    compute_factorials(fact);

    const unsigned long mask_limit = 1U << 16;      /* 65536 subsets */
    for (unsigned long mask = 0; mask < mask_limit; ++mask) {
        if (mask == 0) continue;                  /* skip empty set */
        seq_t sum = (seq_t)0;
        for (unsigned i = 0; i < 16; ++i)
            if (mask & (1U << i))
                sum += fact[i];

        seq_t r = floor_sqrt(sum);
        if (r * r == sum)
            add_square(sum);
    }

    /* Sort and deduplicate */
    if (sq_cnt > 1) {
        qsort(squares, sq_cnt, sizeof(seq_t), cmp_seq);
        size_t uniq = 1;
        for (size_t i = 1; i < sq_cnt; ++i) {
            if (squares[i] != squares[uniq - 1])
                squares[uniq++] = squares[i];
        }
        sq_cnt = uniq;
        seq_t *tmp = (seq_t *)realloc(squares, sq_cnt * sizeof(seq_t));
        if (tmp) squares = tmp;
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    if (n == 0 || n > sq_cnt) return (seq_t)0;
    return squares[n - 1];
}
