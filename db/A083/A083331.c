/*
 * A083331  a(n) = (3*5^n - 2*4^n + 3^n)/2.
 * https://oeis.org/A083331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*5^n - 2*4^n + 3^n)/2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp) {
        if (exp & 1) {
            result *= b;
        }
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p5 = pow_uint((seq_t)5, n);
    seq_t p4 = pow_uint((seq_t)4, n);
    seq_t p3 = pow_uint((seq_t)3, n);

    seq_t temp = 3 * p5;   /* 3*5^n */
    temp -= 2 * p4;        /* -2*4^n */
    temp += p3;            /* +3^n */
    return temp / 2;       /* divide by 2 */
}
