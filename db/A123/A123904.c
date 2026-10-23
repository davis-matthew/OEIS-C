/*
 * A123904  a(n) = n-th integer from among those positive integers with an
 *          exponent of n in their prime-factorizations.
 * https://oeis.org/A123904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * (2n - 1) for n >= 6. For the first five indices:
 *            a(1)=2, a(2)=9, a(3)=27, a(4)=81, a(5)=243
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0)                                    /* offset is 1, guard for 0 */
        return 0;
    if (n < 6) {
        switch (n) {
            case 1: return (seq_t)2;
            case 2: return (seq_t)9;
            case 3: return (seq_t)27;
            case 4: return (seq_t)81;
            case 5: return (seq_t)243;
        }
    }
    seq_t two_power = pow2(n);                       /* 2^n */
    seq_t factor   = (seq_t)((idx_t)2 * n - (idx_t)1);/* (2n-1) */
    return two_power * factor;
}
