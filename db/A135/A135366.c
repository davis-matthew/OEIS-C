/*
 * A135366  a(n) is the smallest nonnegative k such that n divides 2^k + k.
 * https://oeis.org/A135366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest non‑negative k such that n divides 2^k + k
 * Range:     u8 n <= 76, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /* Modulo value as seq_t.  n is always >= 1 for our use. */
    seq_t mod_n = (seq_t)n;

    /* 2^0 mod n */
    seq_t pow = (mod_n == 0) ? 0 : 1 % mod_n;

    /* Iterate k starting from 0. */
    for (idx_t k = 0; ; ++k)
    {
        /* Check if (2^k + k) is divisible by n. */
        if ( ((pow + (seq_t)k) % mod_n) == 0 )
            return (seq_t)k;

        /* Update 2^k to 2^(k+1) modulo n. */
        pow = (pow * 2) % mod_n;
    }
}
