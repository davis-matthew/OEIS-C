/*
 * A217469  Multiplicative order of 5 (mod 5*n + 1).
 * https://oeis.org/A217469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the least m such that 5^m ≡ 1 (mod 5*n+1) */
seq_t a(idx_t n)
{
    /* The modulus is 5*n + 1; compute it in seq_t. */
    seq_t M = (seq_t)5 * (seq_t)n + 1;

    /* Compute 5 mod M and start the search for the multiplicative order. */
    seq_t p = (seq_t)5 % M;
    seq_t exp = 1;

    /* Iterate until p == 1, counting the exponent. */
    while (p != 1)
    {
        p = (seq_t)((uint64_t)p * 5 % M);
        exp++;
    }

    return exp;
}
