/*
 * A086740  Floor(A000041(n)/n).
 * https://oeis.org/A086740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(A000041(n)/n) (partition number divided by n)
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for NULL */
#include <stdlib.h>          /* for malloc, realloc, free */

static seq_t *p_cache = NULL;   /* Cache of partition numbers p(0..max_n_computed) */
static idx_t max_n_computed = 0;/* Highest n for which p_cache[n] is known */

/* Ensure the cache can hold values up to n */
static void ensure_cache(idx_t n)
{
    if (n <= max_n_computed) return;                    /* already large enough */

    idx_t new_size = n + 1;                             /* indices 0..n inclusive */
    seq_t *tmp = (seq_t *)realloc(p_cache, new_size * sizeof(seq_t));
    if (!tmp) return;                                  /* out‑of‑memory, ignore */

    p_cache = tmp;
    /* Zero the newly allocated entries */
    for (idx_t i = max_n_computed + 1; i < new_size; ++i) p_cache[i] = 0;
}

/* Compute partition numbers up to n using the pentagonal theorem */
static void compute_partitions(idx_t n)
{
    if (n <= max_n_computed) return;        /* nothing to do */

    ensure_cache(n);

    if (p_cache[0] == 0) p_cache[0] = 1;    /* base case p(0) = 1 */

    idx_t start = max_n_computed + 1;       /* next n to compute */
    for (idx_t k = start; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t r = 1; ; ++r) {
            idx_t pent1 = r * ((3 * r) - 1) / 2;
            if (pent1 > k) break;
            seq_t add = p_cache[k - pent1];
            if (r & 1) sum += add; else sum -= add;

            idx_t pent2 = r * ((3 * r) + 1) / 2;
            if (pent2 > k) continue;
            add = p_cache[k - pent2];
            if (r & 1) sum += add; else sum -= add;
        }
        p_cache[k] = sum;
    }
    max_n_computed = n;
}

/* Main function; external linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined, but harmless */
    compute_partitions(n);                     /* ensure p_cache[n] exists */
    return p_cache[n] / (seq_t)n;              /* floor division */
}
