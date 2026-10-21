/*
 * A084820  Numbers n such that n, sigma(n) and phi(n) form an integer
 *          triangle, where sigma=A000203 is the divisor sum and phi=A000010
 *          the totient.
 * https://oeis.org/A084820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 9275, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084820: numbers n such that n, sigma(n) and phi(n) form an
 * integer triangle.  Computed by testing consecutive integers. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/calc_phi.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Static state: cache of sequence terms, its capacity, and the
       next integer to test.  Lazy initialisation guarantees the
       sequence can be queried in any order. */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t next = 1;

    /* Ensure enough capacity for the array. */
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap == 0 ? 256 : seq_cap * 2;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        seq = tmp;
        seq_cap = new_cap;
    }

    /* Grow the list until it contains at least n terms. */
    while (seq_len < n) {
        seq_t m     = (seq_t)next;           /* candidate integer */
        seq_t phi   = calc_phi(next);        /* φ(next)   */
        seq_t sigma = sigma_of(next);        /* σ(next)   */

        /* Find the maximum of the three numbers. */
        seq_t max = m;
        if (phi   > max) max = phi;
        if (sigma > max) max = sigma;

        /* Verify triangle inequality: sum of all three > 2*max. */
        seq_t sum = m + phi + sigma;
        if (sum > (max + max)) {
            seq[seq_len++] = m;              /* store term */
            /* Extend storage if simply inserting filled the array. */
            if (seq_len >= seq_cap) {
                size_t new_cap = seq_cap * 2;
                seq_t *tmp2 = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp2) return (seq_t)0;
                seq = tmp2;
                seq_cap = new_cap;
            }
        }
        ++next;
    }

    return seq[n - 1];   /* a(i) is 1‑based */
}
