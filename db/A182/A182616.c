/*
 * A182616  Number of partitions of 2n that contain odd parts.
 * https://oeis.org/A182616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(2n) - p(n) (p(m) = number of partitions of m)
 * Range:     u8 n <= 8, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of partition numbers p(0) … p(max).  The size of the cache
 * (how many partition values are stored) is given by cache_size.  The
 * allocation is grown to the next power of two that is large enough. */
static seq_t *part_cache = NULL;
static idx_t  cache_size = 0;   /* number of computed entries (current size) */

/* Ensure that partition numbers up to N inclusive are computed.
 * N can be 0 or any positive value; the function is Idempotent. */
static void ensure_cache(idx_t N)
{
    if (cache_size > N)     /* already computed */
        return;

    /* Determine new allocation size (next power of two ≥ N+1). */
    idx_t new_cap = (cache_size == 0 ? 1 : cache_size);
    while (new_cap <= N) new_cap <<= 1;

    /* Allocate new buffer and initialise to zero. */
    seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_buf) exit(EXIT_FAILURE);
    for (idx_t i = 0; i < new_cap; ++i) new_buf[i] = 0;

    /* Copy existing values into the new buffer. */
    if (part_cache) {
        for (idx_t i = 0; i < cache_size; ++i)
            new_buf[i] = part_cache[i];
        free(part_cache);
    }
    part_cache = new_buf;

    /* Record the old size to know from where to start recomputing. */
    idx_t old_size = cache_size;
    cache_size   = new_cap;      /* cache now has space for all indices < new_cap */

    /* Ensure p(0) = 1.  If the cache was empty before, set it now. */
    if (old_size == 0) {
        part_cache[0] = 1;
        old_size = 1;            /* first computed index is now 1 */
    }

    /* Compute partition numbers for indices old_size … new_cap‑1
     * using Euler's pentagonal‑number recurrence. */
    for (idx_t n = old_size; n < new_cap; ++n) {
        seq_t pos = 0, neg = 0;          /* positive and negative terms */

        for (idx_t k = 1;; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > n) break;          /* larger pentagonal numbers are useless */
            seq_t term = part_cache[n - g1];
            if (k & 1) pos += term; else neg += term;

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > n) continue;
            term = part_cache[n - g2];
            if (k & 1) pos += term; else neg += term;
        }
        part_cache[n] = pos - neg;        /* p(n) = pos – neg */
    }
}

/* Public OEIS sequence function. */
seq_t a(idx_t n)
{
    /* p(2n) and p(n) are needed, so ensure that both exist. */
    ensure_cache(2 * n);
    return part_cache[2 * n] - part_cache[n];
}
