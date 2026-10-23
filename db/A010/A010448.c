/*
 * A010448  Squares mod 87.
 * https://oeis.org/A010448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A010448: Squares mod 87
   Formula: a(n) = the n-th smallest value of k^2 mod 87 (1 ≤ n ≤ 30) */

#include <stddef.h>
#include <stdlib.h>

/* Comparison function for seq_t, used by qsort. */
static int cmp_seq_t(const void *p, const void *q)
{
    seq_t v1 = *(const seq_t *)p;
    seq_t v2 = *(const seq_t *)q;
    return (v1 > v2) - (v1 < v2);
}

seq_t a(idx_t n)
{
    /* There are exactly 30 distinct quadratic residues modulo 87. */
    static seq_t residues[30];
    static size_t count = 0;

    if (count == 0) {
        /* Compute and store all residues. */
        for (idx_t k = 0; k < 87; ++k) {
            idx_t r = (k * k) % 87;          /* compute k^2 mod 87 in a wide type */
            /* check whether r is already in the list */
            int dup = 0;
            for (size_t i = 0; i < count; ++i) {
                if (residues[i] == (seq_t)r) {
                    dup = 1;
                    break;
                }
            }
            if (!dup) {
                residues[count++] = (seq_t)r;
            }
        }
        /* Sort the residues in ascending order. */
        qsort(residues, count, sizeof(seq_t), cmp_seq_t);
    }

    /* Undefined for n <= 0 or n > 30; return 0 in that case. */
    if (n == 0 || n > count) {
        return (seq_t)0;
    }
    return residues[n - 1];
}
