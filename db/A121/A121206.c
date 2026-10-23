/*
 * A121206  a(n) = (2n)! mod n(2n+1).
 * https://oeis.org/A121206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)! mod (n*(2*n+1))
 * Range:     u8 n <= 13, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes factorial modulo a triangular number without overflow. */
seq_t a(idx_t n)
{
    idx_t two_n = 2 * n;
    seq_t mod = (seq_t)n * ((seq_t)two_n + 1);
    seq_t result = 1;
    for (idx_t i = 1; i <= two_n; ++i)
    {
        unsigned long long prod = (unsigned long long)result * i;
        result = (seq_t)(prod % mod);
    }
    return result;
}
