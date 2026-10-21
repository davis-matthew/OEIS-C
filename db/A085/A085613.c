/*
 * A085613  a(n) = 2^(n-1) + (2 + (-1)^n)^((n-2)/2).
 * https://oeis.org/A085613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) + (2 + (-1)^n)^((n-2)/2) (n ≥ 1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* compute base^exp for arbitrary seq_t type */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp != 0) {
        if (exp & 1)
            result *= base;
        if (exp > 1)
            base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined; return 0 for safety */

    /* 2^(n-1) */
    seq_t term1 = pow_seq((seq_t)2, n - 1);

    /* (2 + (-1)^n)^((n-2)/2) */
    idx_t exponent = (n > 2) ? (n - 2) / 2 : 0;
    seq_t term2 = 1;                /* default for exponent 0 */
    if ((n & 1) == 0 && exponent > 0)   /* even n => base = 3 */
        term2 = pow_seq((seq_t)3, exponent);
    /* odd n => base = 1, term2 remains 1 */

    return term1 + term2;
}
