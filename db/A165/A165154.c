/*
 * A165154  a(n) = 100*a(n-1) + (-9)^(n-1) for n>0, a(0)=0.
 * https://oeis.org/A165154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = 100*a(n-1) + (-9)^(n-1), a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res  = 0;     /* current a(n) */
    seq_t pow9 = 1;     /* 9^(n-1) in magnitude */
    int sign = 1;       /* +1 when exponent is even, -1 when odd */

    for (idx_t i = 1; i <= n; ++i) {
        /* recurrence step: a_i = 100*a_{i-1} + (-9)^(i-1) */
        res = res * (seq_t)100;
        if (sign > 0)
            res += pow9;       /* add +9^(i-1) */
        else
            res -= pow9;       /* subtract 9^(i-1) */

        /* prepare for next iteration: multiply 9^(i-1) by 9 */
        pow9 *= 9;
        sign = -sign;          /* flip sign for next exponent */
    }
    return res;
}
