/*
 * A232195  Product of distinct squares in Z_n (mod n), without the factor 0.
 * https://oeis.org/A232195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all distinct non‑zero quadratic residues
 *            modulo n, i.e. ∏{x² mod n | 1≤x<n, x² mod n≠0} (mod n)
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* by definition a(1)=0 because modulo 1 all residues are 0 */
    if (n == 1) {
        return (seq_t)0;
    }

    seq_t prod = (seq_t)1;
    seq_t n_seq = (seq_t)n;        /* modulus in seq_t type */

    /* Boolean array to remember which residues have already appeared. */
    char *seen = (char *)calloc((size_t)n, 1);
    if (!seen) {
        /* allocation failure – return 0 as a safe fallback */
        return (seq_t)0;
    }

    /* Compute all squares modulo n and multiply each distinct non‑zero square. */
    for (idx_t x = 1; x < n; ++x) {
        seq_t sq = ((seq_t)x * (seq_t)x) % n_seq;
        if (sq == 0) {
            continue;               /* skip the zero square */
        }
        size_t idx = (size_t)sq;    /* sq is guaranteed < n */
        if (!seen[idx]) {
            seen[idx] = 1;
            prod = (prod * sq) % n_seq;
        }
    }

    free(seen);
    return prod;
}
