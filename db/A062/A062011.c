/*
 * A062011  a(n) = 2*tau(n) = 2*A000005(n).
 * https://oeis.org/A062011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * τ(n) where τ(n) = number of divisors of n (OEIS
 *            A000005)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factorises n by trial division up to √n,
   computes τ(n) as the product of (exponent + 1) for each prime
   factor, and returns 2 · τ(n).  All arithmetic is performed in
   type seq_t, which is the target sequence type. */

seq_t a(idx_t n)
{
    /* n ≥ 1 by problem statement; use uint64_t for internal
       integer ops to avoid overflow in the loop condition. */
    uint64_t m = (uint64_t)n;
    seq_t tau = 1;       /* τ(1) = 1 */

    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            uint64_t exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            /* Multiply tau by (exp+1) within seq_t type. */
            tau *= (seq_t)(exp + 1);
        }
    }
    /* Remaining factor > 1 is a prime with exponent 1. */
    if (m > 1)
    {
        tau *= 2;
    }

    /* Return 2 * τ(n).  The multiplication is performed in seq_t. */
    return tau * 2;
}
