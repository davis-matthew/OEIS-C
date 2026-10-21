/*
 * A238622  Number of partitions of n such that floor(n/2) or ceiling(n/2) is
 *          a part.
 * https://oeis.org/A238622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A238622: Number of partitions of n such that floor(n/2) or ceil(n/2) is a part
 *  Formula:
 *    a(n) = p(n/2)                                   for n even
 *    a(n) = p((n+1)/2) + p((n-1)/2) - 1              for n odd
 *
 *  where p(k) is the partition function (number of unrestricted partitions of k).
 *  Partitions containing a part m are in bijection with partitions of n-m, so
 *  the formulas above follow from that observation.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Cache for partition numbers p[0..p_size] */
static seq_t *p_cache = NULL;
static idx_t  p_size  = 0;     /* largest index fully computed in p_cache   */
static size_t p_cap   = 0;     /* allocated capacity of p_cache              */

/* Ensure that the cache contains p[0..max_n] (0 <= max_n). */
static void ensure_partitions(idx_t max_n)
{
    /* ----------  Alloc / grow the cache  ---------- */
    if (p_cache == NULL) {                       /* first use */
        p_cap = (max_n + 1 > 1 ? (size_t)(max_n + 1) : (size_t)1);
        p_cache = (seq_t *)malloc(p_cap * sizeof(seq_t));
        if (p_cache == NULL) exit(EXIT_FAILURE);
        p_cache[0] = (seq_t)1;                   /* p(0) = 1 */
        p_size  = 0;                             /* nothing beyond index 0 yet */
    } else if (p_cap <= (size_t)max_n) {        /* need more space */
        size_t new_cap = (size_t)(max_n + 1);
        seq_t *tmp = (seq_t *)realloc(p_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        p_cache = tmp;
        p_cap = new_cap;
    }

    /* ----------  Compute missing partition values  ---------- */
    for (idx_t n = p_size + 1; n <= max_n; ++n) {
        seq_t sum = 0;
        /* generalized pentagonal numbers: k(3k-1)/2 and k(3k+1)/2 for k=1,2,... */
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > n) break;
            int sign = (k & 1) ? 1 : -1;      /* sign alternates: +,-,+,-,... */
            sum += (sign == 1) ? p_cache[n - g1] : -(p_cache[n - g1]);

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > n) continue;
            sum += (sign == 1) ? p_cache[n - g2] : -(p_cache[n - g2]);
        }
        p_cache[n] = sum;
    }
    p_size = max_n;
}

/*  Public function mandated by the task  */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* behaviour below offset is irrelevant */

    if ((n & 1) == 0) {                  /* n even : a(n) = p(n/2)         */
        idx_t m = n >> 1;                /* m = n/2                         */
        ensure_partitions(m);
        return p_cache[m];
    } else {                              /* n odd  : a(n) = p((n+1)/2)+p((n-1)/2)-1  */
        idx_t m = n >> 1;                /* m = floor(n/2) = (n-1)/2       */
        ensure_partitions(m + 1);
        seq_t res = p_cache[m] + p_cache[m + 1];
        res -= (seq_t)1;
        return res;
    }
}
