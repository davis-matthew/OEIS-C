/*
 * A216469  a(n) = smallest m such that 2n-1 | (2^m+1)/3, or 0 if no such m
 *          exists.
 * https://oeis.org/A216469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the definition of OEIS A216469:  
   a(n) = smallest odd m such that (2^m + 1)/3 is divisible by (2n-1);  
   returns 0 if no such m exists. */

seq_t a(idx_t n)
{
    /* Modulus is 3*(2n-1).  All computations are in seq_t. */
    seq_t two_n_minus1 = (seq_t)n * 2 - 1;
    seq_t mod = (seq_t)3 * two_n_minus1;

    /* Search odd exponents m.  The order of 2 modulo ``mod`` is at most
       φ(mod) ≤ mod-1 ≤ 6n-1, so 12·n+10 iterations always suffice. */
    idx_t max_iter = 12 * n + 10;    /* upper bound on odd m */

    seq_t pow = 2 % mod;            /* 2^1 modulo mod */
    for (idx_t m = 1; m <= max_iter; m += 2)
    {
        /* Check whether (2^m + 1)/3 is divisible by (2n-1). */
        if (((pow + 1) % mod) == 0)
            return (seq_t)m;        /* minimal odd m found */

        /* Next odd exponent: multiply by 2^2 = 4 modulo mod. */
        pow = (pow * 4) % mod;
    }
    return 0;                       /* no solution within bound */
}
