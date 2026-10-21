/*
 * A207701  Number of n X 6 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-3 + n + 15*n^2 + 2*n^3)^2 / 9
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 42, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t sn = (seq_t)n;
    seq_t part = sn
                 + ((seq_t)15 * sn * sn)
                 + ((seq_t)2 * sn * sn * sn)
                 - (seq_t)3;
    seq_t result = part * part / (seq_t)9;
    return result;
}
