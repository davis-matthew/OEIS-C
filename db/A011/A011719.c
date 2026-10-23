/*
 * A011719  A binary m-sequence: expansion of reciprocal of x^8+x^5+x^3+x+1.
 * https://oeis.org/A011719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 0, a(2) = 0, a(3) = 0, a(4) = 0, a(5) = 0, a(6)
 *            = 0, a(7) = 1; For n ≥ 8 : a(n) = a(n‑1) XOR a(n‑3) XOR a(n‑5)
 *            XOR a(n‑8). The sequence is 255‑periodic
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t seq_cache[255];
static int seq_ready = 0;

seq_t a(idx_t n) {
    if (!seq_ready) {
        for (idx_t i = 0; i < 255; ++i) {
            if (i < 8) {
                /* initialize first 8 bits */
                seq_cache[i] = (i == 7) ? (seq_t)1 : (seq_t)0;
            } else {
                /* recurrence over GF(2) */
                seq_cache[i] = seq_cache[i-1] ^ seq_cache[i-3] ^ seq_cache[i-5] ^ seq_cache[i-8];
            }
        }
        seq_ready = 1;
    }
    return seq_cache[n % 255];
}
