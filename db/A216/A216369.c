/*
 * A216369  a(n) = !(n-1) mod n.
 * https://oeis.org/A216369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = !(n-1) mod n (subfactorial of n-1 modulo n)
 * Range:     u8 n <= 289, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* work entirely modulo n */
    seq_t mod = (seq_t)n;
    seq_t prev2 = 1 % mod;   /* !(0) mod n */
    seq_t prev1 = 0;         /* !(1) mod n */

    for (idx_t k = 2; k <= n - 1; ++k)
    {
        seq_t factor = ((seq_t)(k - 1)) % mod;    /* (k-1) mod n      */
        seq_t sum = (prev1 + prev2) % mod;        /* (!k-1 + !k-2) mod n */
        seq_t curr = (factor * sum) % mod;        /* !k mod n          */
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;  /* ! (n-1) mod n */
}
