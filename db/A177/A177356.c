/*
 * A177356  a(n) is the index of the first 0 term in the rumor sequence with
 *          initial 0th term 1 and parameters b = 2 and n.
 * https://oeis.org/A177356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the minimal k > 0 such that the running‑modulus
 *            recurrence z[0] = 1, z[k] = (2 * z[k-1]) mod (k + n) yields z[k]
 *            = 0
 * Range:     u8 n <= 17, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation computes z iteratively until the zero is reached. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t z = 1;          /* z[0] = 1 */
    for (idx_t k = 1; ; ++k) {
        /* modulus for this step is k + n */
        seq_t mod = (seq_t)k + (seq_t)n;
        /* z = 2 * z (mod mod) */
        z = (z << 1) % mod;
        if (z == 0)
            return (seq_t)k;
    }
}
