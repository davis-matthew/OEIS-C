/*
 * A018467  Divisors of 460.
 * https://oeis.org/A018467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 460 in ascending order
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Comparator for qsort on seq_t. */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    return (*x > *y) - (*x < *y);
}

/* Return the n‑th divisor of 460 (1‑based). Undefined for n<1. */
seq_t a(idx_t n)
{
    static seq_t divs[12];
    static bool initialized = false;

    if (!initialized) {
        seq_t tmp[12];
        size_t pos = 0;
        for (int e1 = 0; e1 <= 2; ++e1) {
            seq_t v1 = 1;
            for (int i = 0; i < e1; ++i)
                v1 *= 2;
            for (int e2 = 0; e2 <= 1; ++e2) {
                seq_t v2 = v1;
                if (e2)
                    v2 *= 5;
                for (int e3 = 0; e3 <= 1; ++e3) {
                    seq_t v3 = v2;
                    if (e3)
                        v3 *= 23;
                    tmp[pos++] = v3;
                }
            }
        }
        qsort(tmp, 12, sizeof(seq_t), cmp_seq);
        for (size_t i = 0; i < 12; ++i)
            divs[i] = tmp[i];
        initialized = true;
    }

    if (n < 1 || n > 12)
        return (seq_t)0;
    return divs[n - 1];
}
