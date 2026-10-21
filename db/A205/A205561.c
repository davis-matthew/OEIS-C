/*
 * A205561  Least positive integer k such that n divides (2k)! - (2j)! for
 *          some j in [1,k-1].
 * https://oeis.org/A205561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 753, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = minimal k ≥ 1 such that (2k)! ≡ (2j)! (mod n) for some 1 ≤ j < k */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>   /* for NULL */

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* n≥1 for this sequence */

    uint64_t N = n;                /* small id for array size and indexing */

    /* Allocate a bool array to remember residues seen modulo n */
    uint8_t *seen = (uint8_t *)calloc((size_t)N, sizeof(uint8_t));
    if (!seen) return 0;           /* unlikely in the test suite */

    idx_t k = 1;

    /* f = (2k)! mod n, start with k = 1 */
    seq_t f = (seq_t)2 % (seq_t)n;
    size_t idx_f = (size_t)(uint64_t)f;   /* f < n ensures cast is safe */
    seen[idx_f] = 1;

    while (1) {
        k++;

        /* multiply by (2k-1) and then by (2k), taking the modulus each step */
        f = f * (seq_t)(2*k - 1) % (seq_t)n;
        f = f * (seq_t)(2*k)    % (seq_t)n;

        idx_f = (size_t)(uint64_t)f;
        if (seen[idx_f]) {
            free(seen);
            return (seq_t)k;
        }
        seen[idx_f] = 1;
    }
}
