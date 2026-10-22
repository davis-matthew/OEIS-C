/*
 * A002439  Glaisher's T numbers.
 * https://oeis.org/A002439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (-4)^n - Sum_{k=1..n} (-9)^k*C(2*n+1, 2*k)*a(n-k), a(0) =
 *            1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *cache_a002439 = NULL;
static idx_t cache_size_a002439 = 0;
static seq_t *row_a002439 = NULL;
static idx_t row_cap_a002439 = 0;
static idx_t row_m_a002439 = 0;
static seq_t pow4_a002439 = 1;

seq_t a(idx_t n) {
    if (n < cache_size_a002439) {
        return cache_a002439[n];
    }

    idx_t start = cache_size_a002439;
    idx_t new_size = n + 1;
    if (new_size < 2 * cache_size_a002439) {
        new_size = 2 * cache_size_a002439;
    }

    if (cache_a002439 == NULL) {
        cache_a002439 = alloc_zero(new_size);
    } else {
        cache_a002439 = grow_zero(cache_a002439, cache_size_a002439, new_size);
    }

    idx_t need = 2 * new_size + 2;
    if (row_a002439 == NULL) {
        row_a002439 = alloc_zero(need);
        row_a002439[0] = 1;
        row_a002439[1] = 1;
        row_m_a002439 = 1;
    } else if (need > row_cap_a002439) {
        row_a002439 = grow_zero(row_a002439, row_cap_a002439, need);
    }
    if (need > row_cap_a002439) {
        row_cap_a002439 = need;
    }

    seq_t *c = cache_a002439;
    seq_t *row = row_a002439;

    for (idx_t i = start; i < new_size; i++) {
        /* advance Pascal row to m = 2i+1 */
        while (row_m_a002439 < 2 * i + 1) {
            for (idx_t j = row_m_a002439 + 1; j >= 1; j--) {
                row[j] += row[j - 1];
            }
            row_m_a002439++;
        }

        seq_t pos = 0, neg = 0;
        if (i % 2 == 0) {
            pos = pow4_a002439;
        } else {
            neg = pow4_a002439;
        }

        seq_t p9 = 1;
        for (idx_t k = 1; k <= i; k++) {
            p9 *= 9;
            seq_t t = row[2 * k] * p9 * c[i - k];
            if (k & 1) {
                pos += t;
            } else {
                neg += t;
            }
        }

        c[i] = pos - neg;
        pow4_a002439 *= 4;
    }

    cache_size_a002439 = new_size;
    return c[n];
}
