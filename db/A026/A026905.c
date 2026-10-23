/*
 * A026905  Partial sums of the partition numbers A000041 of the positive
 *          integers.
 * https://oeis.org/A026905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} p(k) -- partial sums of the partition
 *            numbers A000041
 * Range:     u8 n <= 11, u16 n <= 33, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *p_cache = NULL;        /* partition numbers p(0..n) */
static seq_t *sum_cache = NULL;      /* partial sums a(0..n) = sum_{k=1..i} p(k) */
static idx_t cache_cap = 0;          /* number of entries currently allocated (>=0) */

/* ensures that p_cache[0..n] and sum_cache[0..n] are computed */
static void ensure_cap(idx_t n)
{
    if (n < cache_cap)
        return;                       /* already computed */

    /* allocate new capacity, simple grow to n+1 */
    idx_t new_cap = n + 1;
    seq_t *new_p = (seq_t *)realloc(p_cache, new_cap * sizeof(seq_t));
    seq_t *new_sum = (seq_t *)realloc(sum_cache, new_cap * sizeof(seq_t));
    if (!new_p || !new_sum) {
        /* allocation failure – terminate program */
        exit(1);
    }
    p_cache = new_p;
    sum_cache = new_sum;

    /* initialise first element if this is the first allocation */
    if (cache_cap == 0) {
        p_cache[0] = 1;        /* p(0) = 1 by convention */
        sum_cache[0] = 0;      /* a(0) = 0 (unused) */
    }

    /* compute missing terms from old_cap to n */
    idx_t start = (cache_cap == 0) ? 1 : cache_cap;
    for (idx_t i = start; i <= n; ++i) {
        seq_t term_sum = 0;
        /* generalized pentagonal numbers */
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i)
                break;
            if (k & 1)
                term_sum += p_cache[i - g1];
            else
                term_sum -= p_cache[i - g1];

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > i)
                continue;
            if (k & 1)
                term_sum += p_cache[i - g2];
            else
                term_sum -= p_cache[i - g2];
        }
        p_cache[i] = term_sum;
        sum_cache[i] = sum_cache[i - 1] + term_sum;
    }

    cache_cap = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* behaviour for n < 1 is undefined for the sequence */
    ensure_cap(n);
    return sum_cache[n];
}
