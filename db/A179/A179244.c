/*
 * A179244  Numbers that have 4 terms in their Zeckendorf representation.
 * https://oeis.org/A179244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 35, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that have 4 terms in their Zeckendorf representation.
   OEIS A179244: a(n) is the n‑th integer whose Zeckendorf
   representation contains exactly four Fibonacci summands.
   The implementation enumerates all sums of four distinct
   non‑consecutive Fibonacci numbers, stores them in a cached,
   sorted array, and returns the requested element. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Global cache of the sums.  The array is always sorted
   and contains at least the number of elements requested.
   Once a larger cache is built it replaces the previous
   one. */
static seq_t *cache   = NULL;      /* sorted list of sums              */
static size_t cache_len = 0;       /* current number of elements       */

/* Comparator used by qsort.  Works for any unsigned integer
   type, including arbitrary‑precision types when < and > are
   defined. */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

/* Build a cache containing at least target_len elements.
   The algorithm grows the Fibonacci table until the number
   of possible quadruples with the required gaps exceeds
   target_len.  All valid sums are generated, stored,
   sorted, and then the cache is replaced.  The previous
   cache, if any, is freed. */
static void build_cache(idx_t target_len)
{
    /* Determine how many Fibonacci numbers are needed. */
    size_t fib_cnt = 31;          /* start with a safe size */
    while (true) {
        /* Count of valid quadruples with required gaps:
           choose 4 positions from (fib_cnt-3) available spots. */
        size_t avail = fib_cnt >= 3 ? fib_cnt - 3 : 0;
        size_t est = 0;
        if (avail >= 4) {
            est = (avail * (avail - 1) * (avail - 2) * (avail - 3)) / 24;
        }
        if (est >= (size_t)target_len) break;
        fib_cnt *= 2;   /* grow until enough */
    }

    /* Build Fibonacci array.  We stop increasing when the next
       addition would overflow seq_t: if the next value would
       wrap around to a smaller value, we break. */
    seq_t *fib = (seq_t *)malloc(fib_cnt * sizeof(seq_t));
    fib[0] = (seq_t)1;
    fib[1] = (seq_t)2;
    for (size_t i = 2; i < fib_cnt; ++i) {
        seq_t next = fib[i-1] + fib[i-2];
        /* Stop if overflow would occur (next <= fib[i-1]) */
        if (next <= fib[i-1]) { fib_cnt = i; break; }
        fib[i] = next;
    }

    /* Recompute available and estimate of sums after final fib_cnt. */
    size_t avail = fib_cnt >= 3 ? fib_cnt - 3 : 0;
    size_t est_sums = avail >= 4 ?
        (avail * (avail - 1) * (avail - 2) * (avail - 3)) / 24
        : 0;

    /* Allocate array for sums. */
    seq_t *new_cache = (seq_t *)malloc(est_sums * sizeof(seq_t));
    size_t pos = 0;

    /* Enumerate all quadruples i < j < k < l with gaps of at least
       one Fibonacci number in between.  The nested loops encode
       the gap condition automatically. */
    for (size_t i = 0; i + 3 < fib_cnt; ++i) {
        for (size_t j = i + 2; j + 2 < fib_cnt; ++j) {
            seq_t sum_ij = fib[i] + fib[j];
            for (size_t k = j + 2; k + 1 < fib_cnt; ++k) {
                seq_t sum_ijk = sum_ij + fib[k];
                for (size_t l = k + 2; l < fib_cnt; ++l) {
                    new_cache[pos++] = sum_ijk + fib[l];
                }
            }
        }
    }

    /* The generated count should equal est_sums.  Sort the results. */
    if (pos > 1)
        qsort(new_cache, pos, sizeof(seq_t), cmp_seq_t);

    /* Replace old cache (if any). */
    free(cache);
    cache   = new_cache;
    cache_len = pos;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for offset 1 */

    /* Ensure the cache contains at least n elements. */
    if (cache_len < (size_t)n)
        build_cache(n);

    /* After the call above, cache_len >= n. */
    return cache[n - 1];
}
