/*
 * A038198  Numbers k such that k^2 + 7 is a power of 2.
 * https://oeis.org/A038198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   isqrt_seq, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer k such that k^2 + 7 is a power of two.
 * The function loops over exponents m of the power of two term,
 * tests whether 2^m−7 is a perfect square, and returns the
 * k (square root) of the n‑th such occurrence.
 * If n is larger than the number of solutions, 0 is returned.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    idx_t found = 0;
    /* Only a few exponents give perfect squares; 64 is plenty
     * for all known solutions and for 64‑bit/128‑bit types.
     */
    const idx_t MAX_M = 64;
    for (idx_t m = 3; m <= MAX_M; ++m) {
        seq_t val = pow2(m);
        val -= (seq_t)7;           /* 2^m – 7 */
        seq_t s = isqrt_seq(val);  /* floor(sqrt(val)) */
        if (s * s == val) {        /* perfect square? */
            ++found;
            if (found == n) return s;
        }
    }
    return (seq_t)0; /* no n‑th term exists */
}
