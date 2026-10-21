/*
 * A051344  Number of ways of writing n as a sum of 3 positive cubes (counted
 *          naively).
 * https://oeis.org/A051344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 460, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of ordered triples (a,b,c) of positive integers with
   a^3 + b^3 + c^3 = n (counted naively). */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Minimum possible sum is 1^3+1^3+1^3 = 3. */
    if (n < 3) return 0;

    /* Pre‑compute all positive cubes ≤ n.  For n ≤ 10 000 the cube root is
       at most 21, so an array of size 1000 is more than enough. */
    enum { MAX_CUBES = 1000 };
    seq_t cubes[MAX_CUBES];
    idx_t cube_count = 0;

    for (idx_t i = 1; i < MAX_CUBES; ++i) {
        seq_t val = (seq_t)i * (seq_t)i * (seq_t)i;
        if (val > n) break;
        cubes[cube_count++] = val;
    }

    seq_t count = 0;

    for (idx_t i = 0; i < cube_count; ++i) {
        seq_t i3 = cubes[i];
        for (idx_t j = 0; j < cube_count; ++j) {
            seq_t sumij = i3 + cubes[j];
            /* At least 1^3 must remain. */
            if (sumij > n - 1) break;
            seq_t rem = n - sumij;

            /* Check whether rem is one of the pre‑computed cubes. */
            /* Since a(n) ≤ 10 000 the cube set is tiny; a linear scan is fine. */
            for (idx_t k = 0; k < cube_count; ++k) {
                if (cubes[k] == rem) {
                    ++count;
                    break;
                }
            }
        }
    }

    return count;
}
