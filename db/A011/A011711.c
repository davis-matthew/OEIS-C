/*
 * A011711  A binary m-sequence: expansion of reciprocal of
 *          x^8+x^7+x^6+x^5+x^2+x+1.
 * https://oeis.org/A011711
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

/* Recurrence: a(0)=0,a(1)=0,…,a(6)=0,a(7)=1; for n≥8
   a(n) = a(n‑1) ^ a(n‑2) ^ a(n‑5) ^ a(n‑6) ^ a(n‑7) ^ a(n‑8)   (mod 2).
   The sequence is 255‑periodic. */
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t period[255];
    static int ready = 0;
    if (!ready) {
        period[0] = period[1] = period[2] = period[3] =
        period[4] = period[5] = period[6] = (seq_t)0;
        period[7] = (seq_t)1;
        for (idx_t i = 8; i < 255; ++i) {
            period[i] =
                period[i-1] ^ period[i-2] ^ period[i-5] ^ period[i-6] ^ period[i-7] ^ period[i-8];
        }
        ready = 1;
    }
    /* n is idx_t (unsigned); the period length fits in size_t. */
    return period[(size_t)(n % 255)];
}
