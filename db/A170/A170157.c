/*
 * A170157  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 4*3^(n-1) for n >= 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements a fast exponentiation to keep the runtime
   well below 2 seconds for n up to about 10000. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* compute 3^(n-1) using exponentiation by squaring */
    idx_t exp = n - 1;
    seq_t power = (seq_t)1;
    seq_t base  = (seq_t)3;

    while (exp) {
        if (exp & 1ULL)
            power *= base;
        base *= base;
        exp >>= 1;
    }

    return power * (seq_t)4;
}
