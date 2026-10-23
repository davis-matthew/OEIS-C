/*
 * A105612  Number of nonzero quadratic residues (mod n) (cf. A000224).
 * https://oeis.org/A105612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 512, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A105612: number of nonzero quadratic residues mod n.
   The value is the count of distinct r ≠ 0 such that there exists x
   with x^2 ≡ r (mod n).  For n = 1 the result is 0.
   The implementation uses a simple O(n) enumeration of squares a
   time‑per‑call proportional to n, suitable for n up to about 10⁴. */

#include <stdlib.h>
#include <stdint.h>          /* for uint8_t */

seq_t a(idx_t n)
{
    if (n <= 1u) return 0;

    // Allocate a flag array for residues 0 .. n-1
    uint8_t *seen = (uint8_t*)calloc((size_t)n, 1u);
    if (!seen) return 0;                 /* allocation failed */

    seq_t count = 0;

    for (idx_t x = 1; x < n; ++x) {
        seq_t r = ((seq_t)x * (seq_t)x) % n;   /* r is in [0, n-1] */
        if (r != 0 && !seen[(size_t)r]) {
            seen[(size_t)r] = 1;
            ++count;
        }
    }

    free(seen);
    return count;
}
