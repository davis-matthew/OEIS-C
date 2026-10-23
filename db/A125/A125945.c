/*
 * A125945  a(n) is the number of nonnegative integers k less than 10^n such
 *          that the decimal representation of k lacks at least one of digits
 *          1,2, at least one of digits 3,4 and at least one of digits
 *          5,6,7,8,9.
 * https://oeis.org/A125945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 20*7^n - 60*6^n + 85*5^n - 70*4^n + 34*3^n - 9*2^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_u(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    seq_t p2 = pow_u((seq_t)2, n);
    seq_t p3 = pow_u((seq_t)3, n);
    seq_t p4 = pow_u((seq_t)4, n);
    seq_t p5 = pow_u((seq_t)5, n);
    seq_t p6 = pow_u((seq_t)6, n);
    seq_t p7 = pow_u((seq_t)7, n);

    seq_t res = 1;                  /* constant +1 */
    res += p7 * (seq_t)20;          /* 20*7^n */
    res -= p6 * (seq_t)60;          /* -60*6^n */
    res += p5 * (seq_t)85;          /* +85*5^n */
    res -= p4 * (seq_t)70;          /* -70*4^n */
    res += p3 * (seq_t)34;          /* +34*3^n */
    res -= p2 * (seq_t)9;           /* -9*2^n */
    return res;
}
