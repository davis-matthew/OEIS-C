/*
 * A072921  a(1)=1; a(n) = a(n-1) + [sum of all decimal digits present so far
 *          in the sequence].
 * https://oeis.org/A072921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1; a(n) = a(n-1) + sum of digits of all previous terms
 * Range:     u8 n <= 10, u16 n <= 91, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t cap = 0;
    static idx_t max_n = 0;
    static seq_t total = 0; /* sum of digit_sums of terms up to max_n */

    if (n == 0) return 0; /* offset not needed but safe */

    /* Ensure capacity >= n+1 (1‑based indexing) */
    if (n + 1 > cap) {
        size_t new_cap = (cap > 0) ? cap : 1;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        seq = tmp;
        cap = new_cap;
    }

    if (max_n == 0) {
        /* Initialize first term */
        seq[1] = (seq_t)1;
        total = digit_sum(seq[1]);
        max_n = 1;
    }

    for (idx_t i = max_n; i < n; ++i) {
        idx_t next = i + 1;
        seq[next] = seq[i] + total;
        seq_t d = digit_sum(seq[next]);
        total += d;
        max_n = next;
    }

    return seq[n];
}
