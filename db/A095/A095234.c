/*
 * A095234  a(1) = 1, a(n) = n+a(n-1) if n does not divide a(n-1), else a(n) =
 *          n*a(n-1).
 * https://oeis.org/A095234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1, a(n) = n + a(n-1) if n does NOT divide a(n-1); a(n) =
 *            n * a(n-1) otherwise
 * Range:     u8 n <= 17, u16 n <= 115, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the n-th term of sequence A095234.
 * idx_t is an unsigned 64‑bit index type.
 * seq_t is the sequence value type (8, 16, 32, 64 or 128 bits, or an arbitrary‑precision class).
 */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety: offset starts at 1 */
    seq_t val = 1;                  /* a(1) = 1 */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t si = (seq_t)i;        /* cast index to the value type */
        if (val % si == 0)          /* divisible: multiply */
            val = val * si;
        else                        /* not divisible: add */
            val = val + si;
    }
    return val;
}
