/*
 * A167624  a(n) = n^6 mod 32.
 * https://oeis.org/A167624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 32
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 32; as 32 divides any power of two width, this is safe
       even if seq_t is smaller than 32 bits. */
    seq_t n_mod = (seq_t)(n & 31);
    seq_t res = 1;
    /* compute n_mod^6, keeping all intermediate results reduced modulo 32
       by masking the lower 5 bits. */
    for (int i = 0; i < 6; ++i)
    {
        res = (res * n_mod) & 31;
    }
    return res;
}
