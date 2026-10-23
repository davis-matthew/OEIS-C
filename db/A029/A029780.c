/*
 * A029780  Numbers k such that every digit that appears in k also appears at
 *          least once in both k^2 and k^3.
 * https://oeis.org/A029780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 192, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sequential search for integers k such that every digit of k appears also in k^2 and k^3. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"

static unsigned digit_mask(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    unsigned mask = 0;
    for (int i = 0; i < 10; ++i)
        if (cnt[i])
            mask |= 1u << i;
    return mask;
}

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for offset 1; return 0 as placeholder */
        return (seq_t)0;

    static seq_t *values = NULL;   /* stored sequence values */
    static idx_t cap = 0;          /* allocated capacity */
    static idx_t len = 0;          /* number of computed values */
    static seq_t cand = 0;         /* next candidate to test */

    while (len < n) {
        unsigned mask_k = digit_mask(cand);
        seq_t k2 = cand * cand;
        seq_t k3 = k2 * cand;
        unsigned mask_k2 = digit_mask(k2);
        unsigned mask_k3 = digit_mask(k3);

        if ((mask_k & ~mask_k2) == 0 && (mask_k & ~mask_k3) == 0) {
            if (len == cap) {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(values, newcap * sizeof(seq_t));
                values = tmp;
                cap = newcap;
            }
            values[len++] = cand;
        }
        cand++;
    }

    return values[n - 1];
}
