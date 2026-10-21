/*
 * A056222  Image of partition numbers (A000041) under "little Hankel"
 *          transform that sends [c_0, c_1, ...] to [d_0, d_1, ...] where d_n
 *          = c_n^2 - c_{n+1}*c_{n-1}.
 * https://oeis.org/A056222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = p(n+1)^2 - p(n+2)*p(n) where p(k) is the partition
 *            number. Partitions are generated using Euler's pentagonal number
 *            theorem, memoized for reuse across calls
 * Range:     i8 n <= 10, i16 n <= 24, i32 n <= 62, i64 n <= 164,
 *            i128 n <= 494, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc, exit */

static seq_t *part_cache = NULL;   /* memoized partition values */
static idx_t  part_cache_len = 0;   /* number of entries currently stored */

/* Build partition numbers up to up_to (inclusive). */
static void ensure_partitions(idx_t up_to)
{
    if (part_cache_len > up_to)
        return;

    size_t new_len = (size_t)(up_to + 1);
    seq_t *tmp = (seq_t *)realloc(part_cache, new_len * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);          /* insufficient memory */

    part_cache = tmp;

    /* initial value p(0) = 1 */
    if (part_cache_len == 0) {
        part_cache[0] = (seq_t)1;
        part_cache_len = 1;
    }

    for (idx_t k = part_cache_len; k <= up_to; ++k) {
        seq_t sum = (seq_t)0;
        for (idx_t m = 1;; ++m) {
            idx_t g1 = m * (3 * m - 1) / 2;
            if (g1 > k)
                break;
            seq_t t = part_cache[k - g1];
            if (m & 1)   /* odd m -> + */
                sum += t;
            else         /* even m -> - */
                sum -= t;

            idx_t g2 = m * (3 * m + 1) / 2;
            if (g2 > k)
                continue;
            t = part_cache[k - g2];
            if (m & 1)
                sum += t;
            else
                sum -= t;
        }
        part_cache[k] = sum;
    }
    part_cache_len = up_to + 1;
}

/* Little Hankel transform term */
seq_t a(idx_t n)
{
    idx_t need = n + 2;                 /* need p(n), p(n+1), p(n+2) */
    ensure_partitions(need);

    seq_t p_n   = part_cache[n];
    seq_t p_np  = part_cache[n + 1];
    seq_t p_np2 = part_cache[n + 2];

    seq_t prod1 = p_np * p_np;          /* p(n+1)^2 */
    seq_t prod2 = p_np2 * p_n;          /* p(n+2)*p(n) */

    return prod1 - prod2;
}
