/*
 * A069638  "Sorted" sum of two previous terms, beginning with 0,1. "Sorted"
 *          means to sort the digits of the sum in ascending order.
 * https://oeis.org/A069638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = SORT[a(n-1) + a(n-2)] (a(0)=0, a(1)=1)
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 952 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Sort the decimal digits of v in ascending order, discarding leading zeros. */
static seq_t sort_digits(seq_t v)
{
    if (v == 0)
        return 0;

    int cnt[10] = {0};
    seq_t tmp = v;

    while (tmp > 0) {
        unsigned d = (unsigned)(tmp % 10);
        cnt[d]++;
        tmp /= 10;
    }

    seq_t res = 0;
    bool started = false;

    for (int d = 0; d <= 9; ++d) {
        while (cnt[d]-- > 0) {
            if (!started) {
                if (d == 0)
                    continue;          /* skip leading zeros */
                started = true;
            }
            res = res * 10 + d;
        }
    }

    return res;
}

/* Compute the A069638 sequence term for index n.  Uses a static cache for
   arbitrary order of calls. */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *seq = NULL;
    static idx_t size = 0;      /* number of terms stored so far */

    /* initialise base terms if needed */
    if (size == 0) {
        seq = mem_new(seq_t, 2);
        seq[0] = 0;
        seq[1] = 1;
        size = 2;
    }

    /* enlarge cache if necessary */
    if (n >= size) {
        idx_t new_size = size * 2;
        if (new_size <= n)
            new_size = n + 1;
        seq = mem_resize(seq_t, seq, new_size);

        for (idx_t i = size; i <= n; ++i) {
            seq_t sum = seq[i - 1] + seq[i - 2];
            seq[i] = sort_digits(sum);
        }
        size = n + 1;
    }

    return seq[n];
}
