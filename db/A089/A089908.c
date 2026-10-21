/*
 * A089908  Sum of digits of numbers between 0 and (7/9)*(10^n-1).
 * https://oeis.org/A089908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (7*(-22*(-1+10^n)+9*(4+9*10^n)*n))/162
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS A089908 sequence. */
seq_t a(idx_t n)
{
    /* 10^n */
    seq_t ten_pow = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        ten_pow *= (seq_t)10;

    /* 9*n*(4 + 9*10^n) */
    seq_t t3 = (seq_t)9 * (seq_t)n;                 /* 9*n      */
    seq_t inner = (seq_t)4 + (seq_t)9 * ten_pow;    /* 4 + 9*10^n */
    t3 *= inner;                                   /* 9*n*(4+9*10^n) */

    /* 22 + t3 - 22*10^n */
    seq_t diff = t3 + (seq_t)22;
    diff -= (seq_t)22 * ten_pow;

    /* (diff / 162) * 7  -- diff is divisible by 162 */
    seq_t temp = diff / (seq_t)162;
    return temp * (seq_t)7;
}
