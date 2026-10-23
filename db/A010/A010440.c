/*
 * A010440  Squares mod 79.
 * https://oeis.org/A010440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = R[(n-1) mod 40] + 79*floor((n-1)/40) where R =
 *            {0,1,2,4,5,8,9,10,11,13,16,18,19,20,21,22,23,25,26,31,32,36,38,40,42,44,45,46,49,50,51,52,55,62,64,65,67,72,73,76}
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t residues[40] = {
        0, 1, 2, 4, 5, 8, 9,10,11,13,
        16,18,19,20,21,22,23,25,26,31,
        32,36,38,40,42,44,45,46,49,50,
        51,52,55,62,64,65,67,72,73,76
    };
    idx_t pos   = (n-1) % 40;
    idx_t cycle = (n-1) / 40;
    seq_t res   = residues[pos];
    seq_t offset = (seq_t)cycle * (seq_t)79;
    res += offset;
    return res;
}
