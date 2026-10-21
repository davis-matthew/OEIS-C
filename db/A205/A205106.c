/*
 * A205106  Least k such that n divides the k-th difference between distinct
 *          pairs of numbers 3^j-2^j.
 * https://oeis.org/A205106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   mul2_mod, mul3_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation of OEIS A205106:
 * a(n) = least k such that n divides the k‑th difference between distinct pairs
 * of numbers 3^j – 2^j.  The differences are enumerated by increasing j,
 * with i from 1 to j‑1, so that the first difference has k = 1, the second has
 * k = 2, and so on.  For a given n we keep the first occurrence of each
 * residue of 3^j–2^j modulo n.  As soon as a residue repeats the difference
 * between the two corresponding j’s is a multiple of n, and the position
 * in the enumeration of that difference is the desired k.  This uses only
 * the integer operations permitted by the requirements. */
#include <stdlib.h>
#include "../utils/mul2_mod.h"
#include "../utils/mul3_mod.h"

seq_t a(idx_t n)
{
    /* Allocate table of first occurrences of residues, 0 means unseen */
    idx_t *first = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (!first) return 0;            /* allocation failed – but this should not happen */

    seq_t mod = (seq_t)n;
    seq_t p3 = 3 % mod;              /* 3^1 mod n */
    seq_t p2 = 2 % mod;              /* 2^1 mod n */

    /* j = 1: record the first residue */
    seq_t r = (p3 >= p2) ? (p3 - p2) : (p3 + mod - p2);
    first[(size_t)r] = 1;            /* first occurrence at j = 1 */

    idx_t j = 2;                     /* start with j = 2 */
    p3 = mul3_mod(p3, mod);          /* 3^2 mod n */
    p2 = mul2_mod(p2, mod);          /* 2^2 mod n */

    for (;;) {
        r = (p3 >= p2) ? (p3 - p2) : (p3 + mod - p2);

        idx_t i0 = first[(size_t)r];
        if (i0 != 0) {              /* residue seen before – we found the answer */
            /* number of differences processed before this j */
            seq_t before = ((seq_t)(j - 2) * (j - 1)) / 2;
            seq_t k = before + (seq_t)i0;  /* index of the divisible difference */
            free(first);
            return k;
        }

        first[(size_t)r] = j;       /* record first occurrence of this residue */

        /* advance to next j */
        ++j;
        p3 = mul3_mod(p3, mod);
        p2 = mul2_mod(p2, mod);
    }
}
