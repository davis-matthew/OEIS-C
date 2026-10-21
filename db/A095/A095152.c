/*
 * A095152  Number of 3-block covers of a labeled n-set.
 * https://oeis.org/A095152
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (11 - 6*3^n + 7^n) / 6, offset = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*       (from A095152: number of 3‑block covers of a labeled n‑set) */

#include <stdint.h>

/* binary exponentiation for seq_t base, idx_t exponent */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0)
    {
        if (exp & 1U)
            result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* OEIS offset is 2; for n < 2 the value is not defined */
    if (n < 2)
        return 0;

    seq_t pow3 = pow_seq_t((seq_t)3, n);
    seq_t pow7 = pow_seq_t((seq_t)7, n);

    /* compute (11 + 7^n - 6*3^n) / 6 */
    seq_t num = (seq_t)11 + pow7 - (seq_t)6 * pow3;
    return num / 6;
}
