/*
 * A170652  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 19 * 18^(n-1) for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    /* compute 18^(n-1) using exponentiation by squaring */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)18;
    idx_t exp    = n - 1;

    while (exp) {
        if (exp & 1)
            result *= base;
        base   *= base;
        exp   >>= 1;
    }

    /* multiply by 19 for the final value */
    result *= (seq_t)19;
    return result;
}
