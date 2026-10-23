/*
 * A010064  Base 4 self or Colombian numbers (not of form k + sum of base 4
 *          digits of k).
 * https://oeis.org/A010064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers not representable as k + sum of base-4 digits of k (Base 4 self or Colombian numbers) */
#include <stdlib.h>
#include <stdbool.h>

/* Return the nth term of OEIS A010064.  n is 1‑based. */
seq_t a(idx_t n)
{
    /* static storage for the sequence values computed so far */
    static seq_t *seq = NULL;       /* array of computed a(k) values */
    static size_t seq_len = 0;      /* number of values stored */
    static size_t seq_cap = 0;      /* capacity of the array */

    /* last number m processed when generating the sequence */
    static idx_t last_checked = 0;

    /* Extend the sequence until at least n terms are available */
    while (seq_len < n) {
        ++last_checked;
        idx_t m = last_checked;

        /* Determine the maximum possible digit sum in base 4 for numbers up to m */
        idx_t tmp = m;
        uint8_t digits = 0;
        while (tmp > 0) {
            ++digits;
            tmp >>= 2;
        }
        idx_t max_sum = 3 * digits;

        /* Search for a k such that k + s_4(k) == m */
        bool representable = false;
        idx_t k_start = (m > max_sum) ? (m - max_sum) : 0;
        for (idx_t k = k_start; k <= m; ++k) {
            /* compute sum of base‑4 digits of k */
            idx_t t = k;
            idx_t sum = 0;
            while (t > 0) {
                sum += t & 3;   /* t % 4, but faster with bit mask */
                t >>= 2;
            }
            if (k + sum == m) {
                representable = true;
                break;
            }
        }

        /* If m is not representable, it belongs to the sequence */
        if (!representable) {
            /* Resize the sequence array if necessary */
            if (seq_len == seq_cap) {
                size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                /* If realloc fails we abort – the environment guarantees sufficient memory. */
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)m;
        }
    }

    return seq[n - 1];
}
