/*
 * A119646  a(n) = maximum number of partitions of n into 3 parts, each having
 *          the same product.
 * https://oeis.org/A119646
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A119646: a(n) = maximum number of unordered triples (x,y,z) of
   positive integers such that x + y + z = n and x*y*z are equal. */

#include <stdlib.h>
#include <stdint.h>

static int cmp_uint64(const void *p1, const void *p2)
{
    uint64_t v1 = *(const uint64_t *)p1;
    uint64_t v2 = *(const uint64_t *)p2;
    return (v1 < v2) ? -1 : (v1 > v2) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    idx_t max_a = n / 3;
    idx_t tri_cnt = 0;
    for (idx_t a = 1; a <= max_a; ++a) {
        idx_t remaining = n - a;
        idx_t max_b = remaining / 2;
        if (max_b >= a)
            tri_cnt += (max_b - a + 1);
    }

    uint64_t *products = (uint64_t *)malloc((size_t)tri_cnt * sizeof(uint64_t));
    if (!products)
        return (seq_t)0;   /* allocation failure */

    idx_t pos = 0;
    for (idx_t a = 1; a <= max_a; ++a) {
        idx_t remaining = n - a;
        idx_t max_b = remaining / 2;
        for (idx_t b = a; b <= max_b; ++b) {
            idx_t c = n - a - b;          /* c ≥ b by construction */
            uint64_t prod = (uint64_t)a * (uint64_t)b * (uint64_t)c;
            products[pos++] = prod;
        }
    }

    qsort(products, (size_t)pos, sizeof(uint64_t), cmp_uint64);

    idx_t best = 1;
    idx_t current = 1;
    for (idx_t i = 1; i < pos; ++i) {
        if (products[i] == products[i - 1])
            ++current;
        else {
            if (current > best)
                best = current;
            current = 1;
        }
    }
    if (current > best)
        best = current;

    free(products);
    return (seq_t)best;
}
