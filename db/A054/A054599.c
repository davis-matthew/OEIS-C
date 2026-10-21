/*
 * A054599  a(n) = Sum_{d|n} d*2^(n/d - 1).
 * https://oeis.org/A054599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d*2^(n/d - 1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t sum = (seq_t)0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0)
            continue;

        /* divisor d1 = i */
        idx_t d1 = i;
        idx_t e1 = n / d1 - 1;
        seq_t pow2 = (seq_t)1;
        for (idx_t k = 0; k < e1; ++k)
            pow2 = pow2 * (seq_t)2;
        sum = sum + (pow2 * (seq_t)d1);

        /* divisor d2 = n / i */
        idx_t d2 = n / i;
        if (d2 == d1)
            continue;

        idx_t e2 = n / d2 - 1;
        pow2 = (seq_t)1;
        for (idx_t k = 0; k < e2; ++k)
            pow2 = pow2 * (seq_t)2;
        sum = sum + (pow2 * (seq_t)d2);
    }

    return sum;
}
