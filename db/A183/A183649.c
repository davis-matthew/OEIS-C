/*
 * A183649  Number of (n+1) X 7 0..4 arrays with every 2 X 2 subblock summing
 *          to 8.
 * https://oeis.org/A183649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*5^n + 504*4^n + 5796*3^n + 24276*2^n + 47544
 * Range:     u8 none, u16 none, u32 n <= 11, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t pow2 = 1;
    seq_t pow3 = 1;
    seq_t pow4 = 1;
    seq_t pow5 = 1;

    for (idx_t i = 0; i < n; ++i) {
        pow2 *= 2;
        pow3 *= 3;
        pow4 *= 4;
        pow5 *= 5;
    }

    return 5 * pow5 + 504 * pow4 + 5796 * pow3 + 24276 * pow2 + 47544;
}
