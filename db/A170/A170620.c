/*
 * A170620  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1, a(n)=35*34^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t res   = 1;
    seq_t base  = 34;          /* 34 as seq_t */
    idx_t exp   = n - 1;

    while (exp > 0) {
        if (exp & 1)
            res *= base;
        exp >>= 1;
        if (exp == 0)
            break;
        base *= base;
    }

    return res * 35;
}
