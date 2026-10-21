/*
 * A070884  7 + x where x is congruent to {0, 4, 6, 10, 12, 16, 22, 24} mod
 *          30.
 * https://oeis.org/A070884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7 + offset[n mod 8] + 30 * (n / 8) where offset = {0, 4,
 *            6, 10, 12, 16, 22, 24}
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t _A070884_offsets[8] = {
    0, 4, 6, 10, 12, 16, 22, 24
};

seq_t a(idx_t n)
{
    /* number of complete 30‑cycles */
    idx_t cycles = n / 8;
    /* position inside the current cycle */
    idx_t pos = n % 8;

    /* compute the value using 64‑bit arithmetic then cast to seq_t */
    return (seq_t)7
           + _A070884_offsets[pos]
           + (seq_t)cycles * 30;
}
