/*
 * A183664  Number of (n+1) X 2 0..6 arrays with every 2 X 2 subblock summing
 *          to 12.
 * https://oeis.org/A183664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*7^n + 12*6^n + 10*5^n + 8*4^n + 6*3^n + 4*2^n + 2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: binary exponentiation for seq_t values */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* safety: sequence is defined for n >= 1 */

    seq_t pow7 = pow_seq((seq_t)7, n);
    seq_t pow6 = pow_seq((seq_t)6, n);
    seq_t pow5 = pow_seq((seq_t)5, n);
    seq_t pow4 = pow_seq((seq_t)4, n);
    seq_t pow3 = pow_seq((seq_t)3, n);
    seq_t pow2 = pow_seq((seq_t)2, n);

    return (seq_t)7   * pow7
         + (seq_t)12  * pow6
         + (seq_t)10  * pow5
         + (seq_t)8   * pow4
         + (seq_t)6   * pow3
         + (seq_t)4   * pow2
         + (seq_t)2;
}
