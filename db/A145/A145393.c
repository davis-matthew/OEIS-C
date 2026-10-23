/*
 * A145393  Number of inequivalent sublattices of index n in square lattice,
 *          where two sublattices are considered equivalent if one can be
 *          rotated or reflected to give the other, with that rotation or
 *          reflection preserving the parent square lattice.
 * https://oeis.org/A145393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (1 + floor(d/4))
 * Range:     u8 n <= 335, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for n == 0 just in case; the offset starts at 1. */
    if (n == 0)
        return 0;

    seq_t res = 0;
    /* Iterate over divisor pairs (d, n/d) safely without overflow. */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            idx_t other = n / d;

            res += (seq_t)1 + (seq_t)(d / 4);
            if (other != d)
                res += (seq_t)1 + (seq_t)(other / 4);
        }
    }
    return res;
}
