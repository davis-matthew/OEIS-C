/*
 * A031526  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 28.
 * https://oeis.org/A031526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number k such that the continued fraction of sqrt(k) has
 *            even period and, after deleting the last term of the periodic
 *            part, the central term is 28
 * Range:     u8 none, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* maximum expected period length for the numbers we will test */
#define MAX_PERIOD 8192

seq_t a(idx_t n)
{
    static seq_t *list = NULL;      /* array of found numbers */
    static size_t list_cap = 0;    /* capacity of the array          */
    static size_t list_len = 0;    /* current number of entries       */
    static idx_t next_k = 0;        /* next integer to test            */

    /* temporary storage for the period terms of sqrt(k) */
    seq_t terms[MAX_PERIOD];

    /* generate values until we have at least n entries */
    while (list_len < (size_t)n) {
        /* grow the result array if necessary */
        if (list_len == list_cap) {
            size_t new_cap = (list_cap == 0) ? 1024 : list_cap * 2;
            seq_t *tmp = mem_new(seq_t, new_cap);
            if (!tmp) abort();
            if (list) {
                mem_copy(tmp, list, list_len * sizeof(seq_t));
                mem_free(list);
            }
            list = tmp;
            list_cap = new_cap;
        }

        /* test the next integer k */
        ++next_k;
        seq_t D = (seq_t)next_k;
        seq_t a0 = floor_sqrt(D);

        /* skip perfect squares */
        if (a0 * a0 == D) continue;

        /* compute the period of the continued fraction of sqrt(D) */
        idx_t period = 0;
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_prev = a0;

        size_t idx = 0;
        while (true) {
            m = d * a_prev - m;
            d = (D - m * m) / d;
            seq_t a_curr = (a0 + m) / d;

            terms[idx++] = a_curr;
            period++;

            if (a_curr == 2 * a0) break;  /* last term of the period */

            a_prev = a_curr;
        }

        /* check the required condition */
        if ((period & 1) == 0) {                    /* period even */
            size_t half = (size_t)(period / 2);
            if (terms[half - 1] == (seq_t)28) {    /* middle term after deletion */
                list[list_len++] = D;
            }
        }
    }

    return list[n - 1];
}
