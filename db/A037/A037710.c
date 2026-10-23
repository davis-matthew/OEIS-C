/*
 * A037710  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,2,0.
 * https://oeis.org/A037710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(35 * 5^n / 104)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the arbitrary‑precision helper when seq_t is larger than 64 bits.
   For 64‑bit (or smaller) types compute 5^n by repeated multiplication
   – this is safe for n up to 27, which covers all 64‑bit test values. */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* below the offset – harmless */

    seq_t pow5;
    if (sizeof(seq_t) > 8) {           /* arbitrary‑precision */
        pow5 = big_pow((seq_t)5, (seq_t)n);
    } else {                           /* 64‑bit or smaller */
        pow5 = 1;
        for (idx_t i = 0; i < n; ++i) {
            pow5 *= 5;                 /* safe: 5^n ≤ 2^64 for n ≤ 27 */
        }
    }

    seq_t part1 = (pow5 / (seq_t)104) * (seq_t)35;
    seq_t part2 = ((pow5 % (seq_t)104) * (seq_t)35) / (seq_t)104;
    return part1 + part2;
}
