/*
 * A204822  Sum of divisors (A000203) of abundant numbers (A005101).
 * https://oeis.org/A204822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = sigma(A005101(n)) where A005101(n) is the nth abundant
 *            number (sum of proper divisors > n)
 * Range:     u8 n <= 24, u16 n <= 4373, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifndef __cplusplus
/* In C build: seq_t is an unsigned integer type, so we can use malloc/calloc. */
#define FREE_SEQ(p) free(p)
#define ALLOC_SEQ(p, n) (p) = (seq_t *)calloc((size_t)(n), sizeof(seq_t))
#else
/* In C++ build: seq_t may be a class, so use new[] and delete[]. */
#define FREE_SEQ(p) delete [] (p)
#define ALLOC_SEQ(p, n) (p) = new seq_t[(size_t)(n)]
#endif

static idx_t current_limit      = 0;   /* limit used for the last sieve */
static seq_t *abundant_nums      = NULL; /* stored abundant numbers (0‑based) */
static seq_t *abundant_sigma     = NULL; /* stored sigma of abundant numbers */
static size_t abundant_count     = 0;   /* number of abundant numbers stored */

/* recompute all sigma values and abundant numbers up to 'limit' */
static void recompute_up_to(idx_t limit)
{
    /* free previous data */
    if (abundant_nums) FREE_SEQ(abundant_nums);
    if (abundant_sigma) FREE_SEQ(abundant_sigma);

    /* allocate sigma array: sigma[0] unused */
    seq_t *sigma = NULL;
    ALLOC_SEQ(sigma, limit + 1);
    if (!sigma) abort();          /* memory failure */

    /* sieve to fill sigma: for each divisor i add i to its multiples */
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t add = (seq_t)i;
        for (idx_t j = i; j <= limit; j += i) {
            sigma[j] += add;
        }
    }

    /* collect abundant numbers */
    /* the worst case number of abundant numbers <= limit is limit itself,
       so allocate that many slots. */
    ALLOC_SEQ(abundant_nums, limit);
    ALLOC_SEQ(abundant_sigma, limit);
    if (!abundant_nums || !abundant_sigma) abort();

    abundant_count = 0;
    for (idx_t j = 1; j <= limit; ++j) {
        /* sigma[j] > 2*j  <=>  sigma[j] > (seq_t)j * 2 */
        if (sigma[j] > ((seq_t)j * 2)) {
            abundant_nums[abundant_count]   = (seq_t)j;
            abundant_sigma[abundant_count]  = sigma[j];
            ++abundant_count;
        }
    }

    /* free sigma array */
    FREE_SEQ(sigma);

    current_limit = limit;
}

/* ensure that we have at least 'n' abundant numbers computed */
static void ensure_abundant(idx_t n)
{
    if (abundant_count >= (size_t)n) return;

    /* start at a modest limit and double until enough abundant numbers */
    idx_t limit = current_limit ? current_limit : 200;
    while (abundant_count < (size_t)n) {
        limit *= 2;
        recompute_up_to(limit);
    }
}

/* sequence function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 – undefined for n<1 */
    ensure_abundant(n);
    return abundant_sigma[n - 1];
}
