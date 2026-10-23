/*
 * A170665  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = 32 * 31^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;           /* a(0) */

    /* compute 31^(n-1) using binary exponentiation */
    idx_t exp   = n - 1;
    seq_t  res  = 1;        /* result of 31^exp */
    seq_t  base = 31;       /* current power of 31 */

    while (exp)
    {
        if (exp & 1)
            res *= base;    /* res *= base when current bit is 1 */
        base *= base;       /* square the base */
        exp >>= 1;          /* move to next bit */
    }

    return 32 * res;        /* multiply by 32 for a(n) */
}
