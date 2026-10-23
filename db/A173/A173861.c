/*
 * A173861  Expansion of 8/7 in base phi.
 * https://oeis.org/A173861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A173861: digits of 8/7 expressed in base φ (golden ratio base).
   a(1) = 1, and for n ≥ 2 the sequence repeats every 16 digits:
   0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0
   Thus a(n) is 1 only when n ≡ 6 (mod 16). */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    /* digits for positions n = 2 … 17 (period 16) */
    static const seq_t pat[16] = {
        0, 0, 0, 0, 1, 0,
        1, 0, 1, 0, 1, 0,
        0, 0, 0, 0
    };
    idx_t idx = (n - 2) % 16;
    return pat[idx];
}
