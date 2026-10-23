/*
 * A024858  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (natural numbers), t = (Lucas numbers).
 * https://oeis.org/A024858
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊n/2⌋} i * L_{n-i+1}
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *lucas_cache = NULL;
    static idx_t lucas_len = 0;
    static seq_t *a_cache = NULL;
    static idx_t a_len = 0;

    if (n < 2)
        return (seq_t)0;

    /* Extend Lucas cache if necessary */
    if (n >= lucas_len) {
        idx_t old_len = lucas_len;
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(lucas_cache, new_len * sizeof(seq_t));
        if (!tmp) exit(1);
        lucas_cache = tmp;
        idx_t start;
        if (old_len == 0) {
            lucas_cache[0] = (seq_t)2;
            if (new_len > 1) lucas_cache[1] = (seq_t)1;
            start = 2;
        } else if (old_len == 1) {
            if (new_len > 1) lucas_cache[1] = (seq_t)1;
            start = 2;
        } else {
            start = old_len;
        }
        for (idx_t i = start; i < new_len; ++i)
            lucas_cache[i] = lucas_cache[i - 1] + lucas_cache[i - 2];
        lucas_len = new_len;
    }

    /* Extend a-cache if necessary */
    if (n >= a_len) {
        idx_t old_len = a_len;
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
        if (!tmp) exit(1);
        a_cache = tmp;
        /* initialize new entries to 0 */
        for (idx_t i = old_len; i < new_len; ++i)
            a_cache[i] = (seq_t)0;
        idx_t m_start = (old_len > 2 ? old_len : 2);
        for (idx_t m = m_start; m < new_len; ++m) {
            if (a_cache[m] != 0) continue;
            idx_t k = m / 2;
            seq_t sum = (seq_t)0;
            for (idx_t i = 1; i <= k; ++i)
                sum += ((seq_t)i) * lucas_cache[m - i + 1];
            a_cache[m] = sum;
        }
        a_len = new_len;
    }

    return a_cache[n];
}
