/*
 * A010463  Weight distribution of [32,21,6] BCH code.
 * https://oeis.org/A010463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   weight distribution of the extended [32,21,6] BCH code. The
 *            codewords are generated from the 21‑bit message space using the
 *            generator polynomial g(x) = x^10 + x^9 + x^3 + x + 1. For each
 *            message we form the 31‑bit codeword, compute its weight, append
 *            an overall parity bit and count the resulting...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static unsigned popcount32(uint32_t v) {
    unsigned c = 0;
    while (v) {
        v &= v - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n) {
    /* static state */
    static seq_t table[17];
    static uint32_t basis[21];
    static int ready = 0;

    if (!ready) {
        /* generator polynomial g(x) = 1 + x + x^3 + x^9 + x^10  (decimal 1547) */
        const uint32_t gbits = 1547u;
        for (int i = 0; i < 21; ++i) {
            basis[i] = gbits << i;   /* shift produces a 31‑bit row of the generator matrix */
        }
        const idx_t Nmsg = (idx_t)1 << 21;   /* 2^21 messages */

        for (idx_t msg = 0; msg < Nmsg; ++msg) {
            uint32_t w = 0;                     /* 31‑bit codeword */
            for (int i = 0; i < 21; ++i) {
                if (msg & ((idx_t)1 << i))
                    w ^= basis[i];
            }
            unsigned pw = popcount32(w);         /* weight of the 31‑bit word */
            unsigned parity = pw & 1u;           /* overall parity bit */
            unsigned tot = pw + parity;          /* weight after extending */
            unsigned idx = tot >> 1;             /* idx = weight/2  (0 ≤ idx ≤ 16) */
            table[idx] += (seq_t)1;
        }
        ready = 1;
    }

    if (n > 16)
        return (seq_t)0;
    return table[(size_t)n];
}
