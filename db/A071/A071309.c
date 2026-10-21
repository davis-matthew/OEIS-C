/*
 * A071309  a(n) = (1/2) * (number of n X n 0..10 matrices with MM' mod 11 =
 *          I).
 * https://oeis.org/A071309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k+1) = 11^k * ∏_{i=0}^{k-1} (11^(2k) - 11^(2i)) a(2k) = (11^k
 *            + (-1)^(k+1)) * ∏_{i=1}^{k-1} (11^(2k) - 11^(2i))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n=0, return 0 */

    const seq_t ONE = (seq_t)1;
    const seq_t VAL = (seq_t)11;
    seq_t result = ONE;

    if ((n & 1U) != 0) {          /* odd: n = 2k+1 */
        idx_t k = (n - 1) / 2;

        /* 11^k */
        seq_t powk = ONE;
        for (idx_t i = 0; i < k; ++i)
            powk *= VAL;

        /* 11^(2k) */
        seq_t pow2k = ONE;
        for (idx_t i = 0; i < 2 * k; ++i)
            pow2k *= VAL;

        /* product over i = 0 .. k-1 of (pow2k - 11^(2i)) */
        seq_t pow2i = ONE;            /* 11^(0) */
        result = powk;
        for (idx_t i = 0; i < k; ++i) {
            result *= (pow2k - pow2i);
            pow2i *= VAL * VAL;       /* multiply by 11^2 for next i */
        }
    } else {                       /* even: n = 2k */
        idx_t k = n / 2;

        /* 11^k */
        seq_t powk = ONE;
        for (idx_t i = 0; i < k; ++i)
            powk *= VAL;

        /* coefficient (11^k + (-1)^(k+1)) */
        if ((k & 1U) == 0)          /* k even => (-1)^(k+1) = -1 */
            result = powk - ONE;
        else                        /* k odd  => (-1)^(k+1) = +1 */
            result = powk + ONE;

        if (k > 1) {
            /* 11^(2k) */
            seq_t pow2k = ONE;
            for (idx_t i = 0; i < 2 * k; ++i)
                pow2k *= VAL;

            /* product over i = 1 .. k-1 of (pow2k - 11^(2i)) */
            seq_t pow2i = VAL * VAL;   /* 11^(2*1) */
            for (idx_t i = 1; i < k; ++i) {
                result *= (pow2k - pow2i);
                pow2i *= VAL * VAL;   /* multiply by 11^2 for next i */
            }
        }
    }
    return result;
}
