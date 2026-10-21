/*
 * A071782  Sum of distinct squares in Z_n (mod n).
 * https://oeis.org/A071782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum of distinct quadratic residues modulo n) modulo n
 * Range:     u8 n <= 33, u16 n <= 870, u32 n <= 7661, u64 n <= 7661,
 *            u128 n <= 7661, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    /* allocate a boolean array to mark seen residues */
    bool *seen = (bool *)malloc((size_t)n);
    if (!seen) return 0;           /* fallback on allocation failure */
    for (idx_t i = 0; i < n; ++i) seen[i] = false;

    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t r = ((seq_t)i * (seq_t)i) % (seq_t)n;
        if (!seen[(size_t)r]) {
            seen[(size_t)r] = true;
            sum += r;
        }
    }
    free(seen);
    return sum % (seq_t)n;
}
