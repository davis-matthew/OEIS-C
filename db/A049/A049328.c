/*
 * A049328  Numbers whose prime factors (taken with multiplicity) sum to 20.
 * https://oeis.org/A049328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   gen_products
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose prime factors (with multiplicity) sum to 20
   This implementation enumerates all integers n such that
   Σ p_i · e_i = 20 (p_i prime, e_i ≥ 1), sorts them, and
   returns the nth smallest value. */

#include <stdlib.h>
#include "../utils/gen_products.h"

/* Comparator for qsort on seq_t */
static int
seq_t_cmp(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    return *x > *y;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Persistent cache */
    static seq_t values[20000];
    static size_t count = 0;

    /* Build cache lazily */
    if (count == 0) {
        gen_products(0, 0, 1, values, &count);
        qsort(values, count, sizeof(seq_t), seq_t_cmp);
    }

    /* Return nth term (1‑based index) */
    if (n == 0 || n > count)
        return 0;   /* undefined for out of bounds */
    return values[(size_t)n - 1];
}
