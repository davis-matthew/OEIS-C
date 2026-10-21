/*
 * A204934  Least k! such that n divides k!-j! for some j<k.
 * https://oeis.org/A204934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 36, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A204934: Least k! such that n divides k! - j! for some j < k.
 *
 * The algorithm works by iterating factorials modulo n and
 * detecting the first repetition of a residue.  The first k for which
 * the current residue has already appeared equals the desired value.
 * The exact factorial k! is maintained in a seq_t variable and
 * returned once repetition occurs.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static inline idx_t mod_mul(idx_t a, idx_t b, idx_t mod) {
    /* use 128-bit arithmetic to avoid overflow of 64-bit product */
    return (idx_t)((__int128)a * b % mod);
}

seq_t a(idx_t n) {
    idx_t mod = n;                     /* modulus for residues */
    /* Allocate array to record the first position of each residue */
    idx_t *seen = (idx_t *)malloc((size_t)mod * sizeof(idx_t));
    if (!seen) return 0;               /* out of memory or n==0 (not expected) */

    memset(seen, 0, (size_t)mod * sizeof(idx_t));

    /* Residue of 1! modulo n, and first factorial value */
    idx_t residue = 1 % mod;           /* for k = 1 */
    seen[residue] = 1;
    seq_t factorial = (seq_t)1;        /* 1! */

    /* Iterate k = 2, 3, ... until a residue repeats */
    for (idx_t k = 2; ; ++k) {
        /* Exact factorial */
        factorial *= (seq_t)k;
        /* Update residue: (residue * k) mod n */
        residue = mod_mul(residue, k, mod);

        /* If this residue has been seen before, return current factorial */
        if (seen[residue]) {
            free(seen);
            return factorial;
        }

        /* Record the first occurrence of this residue */
        seen[residue] = k;
    }
}
