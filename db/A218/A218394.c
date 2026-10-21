/*
 * A218394  Numbers such that sum(i<=n) binomial(n,i)*binomial(2*n-2*i, n-i)
 *          is not divisible by 5.
 * https://oeis.org/A218394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - 1 + 2*sum_{i=1}^{n} 5^(i-1)*floor((2*n-1)/3^i)
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 2;
    base -= 1;                       /* base = 2*n - 1 */
    seq_t sum = 0;
    seq_t pow5 = 1;                  /* 5^(i-1) */
    seq_t pow3 = 3;                  /* 3^i */
    while (pow3 <= base)
    {
        seq_t term = (base / pow3) * pow5;
        sum += term;
        pow5 *= 5;
        pow3 *= 3;
    }
    return base + (sum << 1);        /* base + 2*sum */
}
