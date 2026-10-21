/*
 * A204694  a(n) = n^n (mod 8).
 * https://oeis.org/A204694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n (mod 8)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case n = 0: 0^0 is taken as 1 in this sequence */
    if (n == 0)
        return (seq_t)1;

    /* Reduce the base modulo 8 */
    uint8_t b = (uint8_t)(n & 7); /* n % 8 */

    switch (b) {
    case 1:                         /* 1^k ≡ 1 (mod 8) */
        return (seq_t)1;

    case 3:                         /* 3^k ≡ 3 if k odd, 1 if k even */
        return (seq_t)((n & 1) ? 3 : 1);

    case 5:                         /* 5^k ≡ 5 if k odd, 1 if k even */
        return (seq_t)((n & 1) ? 5 : 1);

    case 7:                         /* 7^k ≡ 7 if k odd, 1 if k even */
        return (seq_t)((n & 1) ? 7 : 1);

    case 2:                         /* 2^1 = 2, 2^2 = 4, 2^k = 0 for k ≥ 3 */
        return (seq_t)((n == 2) ? 4 : 0);

    case 4:                         /* 4^k = 0 for k ≥ 2 (k = n ≥ 4 here) */
        return (seq_t)0;

    case 6:                         /* 6^k = 0 for k ≥ 3 (k = n ≥ 6 here) */
        return (seq_t)0;

    default: /* b == 0 for n > 0 gives 0^k = 0 (k > 0) */
        return (seq_t)0;
    }
}
