/*
 * A023729  Numbers with no 2's in their base-5 expansion.
 * https://oeis.org/A023729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = value obtained by writing (n-1) in base-4, replacing each
 *            digit d (0<=d<4) by d if d<2 else d+2, and interpreting the
 *            resulting base-5 digits as a base-10 integer
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t idx = n - 1;
    seq_t val = 0;
    seq_t pow5 = 1;
    static const int map[4] = {0, 1, 3, 4};
    while (idx > 0) {
        int d = (int)(idx % 4);
        val += (seq_t)map[d] * pow5;
        pow5 *= 5;
        idx /= 4;
    }
    return val;
}
