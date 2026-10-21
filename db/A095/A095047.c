/*
 * A095047  An example of a (v,k,lambda)=(107,53,26) cyclic difference set.
 * https://oeis.org/A095047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = n-th smallest nonzero quadratic residue modulo 107 * The
 *            sequence consists of the 53 distinct residues {1^2,2^2,…,(53)^2}
 *            mod 107, * sorted in increasing order
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t residues[53];
static int residues_initialized = 0;

static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t v1 = *(const seq_t *)p1;
    const seq_t v2 = *(const seq_t *)p2;
    return (v1 > v2) - (v1 < v2);
}

seq_t a(idx_t n)
{
    if (!residues_initialized) {
        for (idx_t i = 1; i <= 53; ++i) {
            seq_t val = (seq_t)i * (seq_t)i % 107;
            residues[i - 1] = val;
        }
        qsort(residues, 53, sizeof(seq_t), cmp_seq);
        residues_initialized = 1;
    }

    if (n == 0) return 0;                    /* undefined for n<1, safe return */

    /* Wrap around if n > 53 to keep function defined for all n ≥ 1 */
    return residues[(n - 1) % 53];
}
