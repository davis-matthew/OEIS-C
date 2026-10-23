/*
 * A028796  Nonsquares mod 83.
 * https://oeis.org/A028796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 83 * floor((n-1)/41) + nonres[ (n-1) mod 41 ], where
 *            nonres is the sorted list of nonsquare residues modulo 83
 * Range:     u8 n <= 41, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset starts at 1 */

    static const seq_t nonres[41] = {
        2,5,6,8,13,14,15,18,19,20,
        22,24,32,34,35,39,42,43,45,46,
        47,50,52,53,54,55,56,57,58,60,
        62,66,67,71,72,73,74,76,79,80,82
    };

    idx_t r = (n - 1) % 41;
    idx_t b = (n - 1) / 41;
    seq_t val = (seq_t)b * (seq_t)83 + nonres[r];
    return val;
}
