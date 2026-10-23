/*
 * A194451  Partition numbers of positive integers and positive integers
 *          interleaved.
 * https://oeis.org/A194451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n-1) = p(n), a(2n) = n where p(n) is the partition number of
 *            n. The partition numbers are computed with the pentagonal number
 *            recurrence: p(0) = 1 p(n) = Σ_{k=1..} (-1)^{k-1} [p(n-g_k) +
 *            p(n-g'_k)], with g_k = k(3k-1)/2, g'_k = k(3k+1)/2 (g_k <= n)
 * Range:     u8 n <= 32, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/to_seq.h"

/* Helper: convert idx_t to seq_t safely (seq_t assumed unsigned integer) */

/* Static cache for partition numbers */
static seq_t *p_cache       = NULL;   /* array of p[0..p_cache_size-1] */
static idx_t  p_cache_size = 0;       /* number of values stored */
static void init_p_cache(void) {
    if (p_cache == NULL) {
        /* allocate space for p(0) and set p(0)=1 */
        p_cache = (seq_t*)malloc(sizeof(seq_t) * 1);
        p_cache[0] = (seq_t)1;
        p_cache_size = 1;
    }
}

/* Ensure cache contains partition numbers up to n (inclusive) */
static void ensure_partitions(idx_t n) {
    if (p_cache_size > n)  return;          /* already have up to n */
    /* resize */
    if (p_cache_size == 0) init_p_cache();  /* safety */
    seq_t *new_cache = (seq_t*)realloc(p_cache, sizeof(seq_t) * (n + 1));
    if (!new_cache) {                        /* allocation failed */
        abort();                              /* fail fast */
    }
    p_cache = new_cache;
    /* compute new values from current size to n */
    for (idx_t m = p_cache_size; m <= n; ++m) {
        seq_t sum_pos = 0, sum_neg = 0;
        idx_t k = m;
        for (idx_t j = 1; ; ++j) {
            /* generalized pentagonal numbers */
            idx_t gp1 = j * (3 * j - 1) / 2;
            if (gp1 > k) break;           /* both will be > k afterwards */
            seq_t term = p_cache[k - gp1];
            if (j & 1) sum_pos += term;   /* sign + */
            else       sum_neg += term;   /* sign - */

            idx_t gp2 = j * (3 * j + 1) / 2;
            if (gp2 > k) continue;
            term = p_cache[k - gp2];
            if (j & 1) sum_pos += term;
            else       sum_neg += term;
        }
        /* final partition number */
        p_cache[m] = sum_pos - sum_neg;
    }
    p_cache_size = n + 1;
}

seq_t a(idx_t n) {
    if (n == 0)  /* sequence is 1-indexed; return 0 for safety */
        return (seq_t)0;
    if ((n & 1ULL) == 0ULL) {             /* even index */
        return to_seq(n / 2);
    } else {                               /* odd index: compute partition */
        idx_t m = (n + 1) / 2;            /* n = 2m-1  =>  m = (n+1)/2 */
        ensure_partitions(m);
        return p_cache[m];
    }
}
