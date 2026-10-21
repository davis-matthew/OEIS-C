/*
 * A202857  Value of the one-dimensional radius 2 totalistic 2-color cellular
 *          automaton rule 20 after 1 step, with initial condition n.
 * https://oeis.org/A202857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = odd part of the one‑dimensional radius‑2 totalistic
 *            2‑color cellular automaton rule 20 after 1 step, with initial
 *            pattern the binary representation of n. The algorithm simulates
 *            the CA for one step, interprets the resulting binary pattern as
 *            an integer, and removes all factors of 2
 * Range:     u8 n <= 47, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* index 0 is unused, but for safety */

    /* Count binary digits of n.  We need at least one digit for n=1. */
    idx_t tmp = n;
    size_t len = 0;
    while (tmp) { ++len; tmp >>= 1; }
    if (len == 0) len = 1;            /* for n==0, handled above, but keep */

    /* The output pattern has length len + 4 (two zeros on each side). */
    const size_t outlen = len + 4;
    /* maximal len for a 64‐bit index is 64, so outlen <= 68. */
    uint8_t cur[68]   = {0};   // current pattern (with padding)
    uint8_t out[68]   = {0};   // next pattern

    /* Place the original bits into cur, offset by 2 to leave two zeros
       on the left.  cur[2..2+len-1] = bits of n (MSB first). */
    for (size_t i = 0; i < len; ++i) {
        cur[2 + i] = (n >> (len - 1 - i)) & 1u;
    }

    /* Compute next pattern according to rule 20:
       a cell becomes 1 iff exactly 2 or 4 of the five cells in its
       neighborhood are 1. */
    for (size_t pos = 0; pos < outlen; ++pos) {
        unsigned sum = 0;
        for (int k = -2; k <= 2; ++k) {
            size_t idx = pos + k;
            if (idx < outlen) sum += cur[idx];
        }
        out[pos] = (sum == 2 || sum == 4) ? 1u : 0u;
    }

    /* Interpret 'out' as a binary integer, most significant bit first. */
    seq_t val = 0;
    for (size_t i = 0; i < outlen; ++i) {
        val = (val << 1) | (seq_t)out[i];
    }

    /* Remove all factors of 2 (odd part). */
    while ((val & 1) == 0 && val != 0) {
        val >>= 1;
    }
    return val;
}
