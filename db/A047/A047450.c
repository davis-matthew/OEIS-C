/*
 * A047450  Numbers that are congruent to {0, 1, 2, 3, 5, 6} mod 8.
 * https://oeis.org/A047450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numbers congruent to {0,1,2,3,5,6} modulo 8. Closed form
 *            derived from OEIS formulas: a(6k) = 8k-2 a(6k-1) = 8k-3 a(6k-2)
 *            = 8k-5 a(6k-3) = 8k-6 a(6k-4) = 8k-7 a(6k-5) = 8k-8
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation directly from the above formulas. */
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t r = n % 6;          /* residue of n modulo 6 */
    if (r == 0) {             /* n is a multiple of 6: a(6k) */
        idx_t k = n / 6;
        return (seq_t)8 * (seq_t)k - 2;
    } else {                  /* n ≡ 1..5 (mod 6) */
        idx_t k = (n + 5) / 6; /* ceil(n/6) */
        switch (r) {
            case 1: return (seq_t)8 * (seq_t)k - 8; /* a(6k-5) */
            case 2: return (seq_t)8 * (seq_t)k - 7; /* a(6k-4) */
            case 3: return (seq_t)8 * (seq_t)k - 6; /* a(6k-3) */
            case 4: return (seq_t)8 * (seq_t)k - 5; /* a(6k-2) */
            case 5: return (seq_t)8 * (seq_t)k - 3; /* a(6k-1) */
            default: return 0; /* unreachable, keeps compiler happy */
        }
    }
}
