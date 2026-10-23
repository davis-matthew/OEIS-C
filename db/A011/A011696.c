/*
 * A011696  A binary m-sequence: expansion of reciprocal of
 *          x^8+x^7+x^6+x^5+x^4+x^3+1.
 * https://oeis.org/A011696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-3) xor a(n-4) xor a(n-5) xor a(n-6) xor a(n-7) xor a(n-8)
 * seed: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=0, a(7)=1
 * The sequence is 85‑periodic.
 */
#include <stdbool.h>

static seq_t seq[85];
static bool seq_ready = false;

seq_t a(idx_t n) {
    if (!seq_ready) {
        /* initialize the first 8 terms of the sequence */
        seq[0] = 0;
        seq[1] = 0;
        seq[2] = 0;
        seq[3] = 0;
        seq[4] = 0;
        seq[5] = 0;
        seq[6] = 0;
        seq[7] = 1;
        /* compute the remaining terms up to period 85 */
        for (idx_t i = 8; i < 85; ++i) {
            seq[i] = seq[i-3] ^ seq[i-4] ^ seq[i-5] ^ seq[i-6] ^ seq[i-7] ^ seq[i-8];
        }
        seq_ready = true;
    }
    /* period 85 */
    idx_t idx = n % 85;
    return seq[idx];
}
