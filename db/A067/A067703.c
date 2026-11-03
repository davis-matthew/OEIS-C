/*
 * A067703  Terms in the decimal expansion of 1/(7*5^n) before the block of
 *          decimals 142857 (the period of 1/7) appears.
 * https://oeis.org/A067703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor( (2^n * 10^k) / 7 ), * where k = * 4 if n mod 3 =
 *            1, * 2 if n mod 3 = 2, * 0 if n mod 3 = 0 * This matches the
 *            OEIS sequence A067703
 * Range:     u8 none, u16 n <= 3, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* a(0) is undefined; return 0 for safety */
    if (n == 0) {
        return (seq_t)0;
    }

    /* 2^n */
    seq_t pow2 = ipow_idx((seq_t)2, n);

    /* 10^k depending on n mod 3 */
    seq_t pow10k;
    switch (n % 3) {
        case 1:
            pow10k = pow10((idx_t)4); /* 10^4 = 10000 */
            break;
        case 2:
            pow10k = pow10((idx_t)2); /* 10^2 = 100  */
            break;
        default: /* n % 3 == 0 */
            pow10k = (seq_t)1;
    }

    /* (2^n * 10^k) / 7 */
    seq_t numerator = pow2 * pow10k;
    seq_t result   = numerator / (seq_t)7;
    return result;
}
