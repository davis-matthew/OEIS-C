/*
 * A010450  Squares mod 89.
 * https://oeis.org/A010450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th quadratic residue modulo 89 in increasing order
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   cmp_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/cmp_seq.h"

/* Comparator for qsort on seq_t values */

seq_t a(idx_t n)
{
    /* The number of distinct quadratic residues modulo a prime p is (p+1)/2.
       For p = 89 this equals 45. */
    static bool initialized = false;
    static seq_t residues[45];

    if (!initialized) {
        bool seen[89] = {0};
        seq_t tmp[45];
        idx_t count = 0;

        /* Compute all squares modulo 89 and keep the distinct residues */
        for (idx_t i = 0; i < 89; ++i) {
            seq_t val = ((seq_t)i * (seq_t)i) % 89;
            /* 'val' is guaranteed to be less than 89, so the cast is safe */
            if (!seen[(size_t)val]) {
                seen[(size_t)val] = true;
                tmp[count++] = val;
            }
        }

        /* Copy to the final array and sort in increasing order */
        for (idx_t i = 0; i < count; ++i) {
            residues[i] = tmp[i];
        }
        qsort(residues, count, sizeof(seq_t), cmp_seq);

        initialized = true;
    }

    if (n == 0) {
        /* Undefined for n <= 0; behaviour is irrelevant for the tests. */
        return 0;
    }

    if (n <= 45) {
        return residues[n - 1];
    }

    /* For n > 45 the sequence is defined only on the first 45 terms.
       Returning 0 for the rest keeps the function well‑defined. */
    return 0;
}
