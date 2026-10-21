/*
 * A216602  Number of ways to write n as a product with all factors <= n/6.
 * https://oeis.org/A216602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into factors
 *            (each > 1) that are all <= n/6. For n > 5 this equals A066032(n,
 *            floor(n/6)). The implementation counts factorizations by a
 *            depth‑first search with non‑decreasing factor order to avoid
 *            duplicates
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t count_factorizations(idx_t n, idx_t min_factor, idx_t limit)
{
    if (n == 1)            /* a complete factorization has been formed */
        return 1;
    seq_t total = 0;
    /* iterate over admissible factors in non‑decreasing order */
    for (idx_t f = min_factor; f <= limit && f <= n; ++f)
        if (n % f == 0)
            total += count_factorizations(n / f, f, limit);
    return total;
}

seq_t a(idx_t n)
{
    if (n <= 5)                 /* offset 1: first term is a(1) */
        return 0;
    idx_t limit = n / 6;        /* integer division gives floor(n/6) */
    return count_factorizations(n, 2, limit);
}
