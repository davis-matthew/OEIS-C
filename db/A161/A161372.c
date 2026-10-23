/*
 * A161372  In Ulam's spiral starting at 101, take the elements not used so
 *          far from the two spokes SW, NE, SW, NE, SW, NE ...
 * https://oeis.org/A161372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n‑1) = 4n² + 2n + 101 * a(2n) = 4n² – 10n + 107 * (n ≥ 1,
 *            integer)
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute m = ⌊(n+1)/2⌋, which equals n/2 for even n
       and (n+1)/2 for odd n.  All operations are performed
       in seq_t to avoid overflow. */
    seq_t m = (seq_t)((n + 1) >> 1);

    if ((n & 1) == 0)           /* even n: use a(2m) */
    {
        return (4 * m * m) - (10 * m) + 107;
    }
    else                        /* odd n:   use a(2m‑1) */
    {
        return (4 * m * m) + (2 * m) + 101;
    }
}
