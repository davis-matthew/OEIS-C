/*
 * A172398  Number of partitions of n into the sum of two refactorable numbers
 *          (A033950).
 * https://oeis.org/A172398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of pairs (i,j), 1≤i≤j≤n, i+j=n, 
 * where i and j are refactorable numbers (i % d(i) == 0).
 *
 * The implementation precomputes divisor counts up to the largest n
 * requested so far using a simple sieve.
 * Each call to a(idx_t n) runs in O(n) time for the final summation.
 *
 * seq_t and idx_t are supplied by the build system.
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* cache of divisor counts */
static uint16_t *div_cnt = NULL;
static idx_t div_cache_size = 0;

/* ensure div_cnt has values up to max_n (inclusive) */
static void ensure_divisor_cache(idx_t max_n)
{
    if (max_n <= div_cache_size)
        return;

    /* extend the cache */
    idx_t new_size = max_n;
    uint16_t *new_cnt = (uint16_t *)realloc(div_cnt, (new_size + 1) * sizeof(uint16_t));
    if (!new_cnt) {
        /* allocation failed – abort; the environment will handle it */
        abort();
    }
    div_cnt = new_cnt;

    /* zero initialize the new part */
    memset(div_cnt, 0, (new_size + 1) * sizeof(uint16_t));

    /* sieve for divisor counts */
    for (idx_t i = 1; i <= new_size; ++i) {
        for (idx_t j = i; j <= new_size; j += i) {
            div_cnt[j] += 1;
        }
    }

    div_cache_size = new_size;
}

/* a(idx_t n) – number of partitions of n into two refactorable numbers */
seq_t a(idx_t n)
{
    if (n < 1)          /* offset is 1; for n=0 return 0 */
        return 0;
    ensure_divisor_cache(n);

    seq_t count = 0;
    idx_t half = n / 2;
    for (idx_t i = 1; i <= half; ++i) {
        idx_t j = n - i;
        /* refactorable iff i % d(i) == 0 */
        if ((i % div_cnt[i] == 0) && (j % div_cnt[j] == 0))
            ++count;
    }
    return count;
}
