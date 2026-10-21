/*
 * A236454  Smallest number not dividing n^2.
 * https://oeis.org/A236454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer k ≥ 2 such that k does not divide n^2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = 2;
    while (1)
    {
        idx_t mod = n % k;
        // Compute (n % k)^2 modulo k safely using 128‑bit arithmetic.
        unsigned __int128 sq = (unsigned __int128)mod * mod;
        if ((sq % (unsigned __int128)k) != 0u)
        {
            return (seq_t)k;
        }
        ++k;
    }
}
