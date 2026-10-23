/*
 * A022535  Nexus numbers (n+1)^19 - n^19.
 * https://oeis.org/A022535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^19 - n^19
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 9, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const int EXP = 19;
    seq_t np1 = (seq_t)n + 1;
    seq_t n_val = (seq_t)n;
    seq_t r1 = 1;
    seq_t r2 = 1;
    int i;

    for (i = 0; i < EXP; ++i) {
        r1 *= np1;
        r2 *= n_val;
    }
    return r1 - r2;
}
