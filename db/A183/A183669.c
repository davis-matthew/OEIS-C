/*
 * A183669  Number of (n+1) X 7 0..6 arrays with every 2 X 2 subblock summing
 *          to 12.
 * https://oeis.org/A183669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*7^n + 756*6^n + 9660*5^n + 48552*4^n + 142632*3^n +
 *            280140*2^n + 341796
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 20, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2 = 1, pow3 = 1, pow4 = 1, pow5 = 1, pow6 = 1, pow7 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= 2;
        pow3 *= 3;
        pow4 *= 4;
        pow5 *= 5;
        pow6 *= 6;
        pow7 *= 7;
    }
    seq_t result = 0;
    result += (seq_t)7 * pow7;
    result += (seq_t)756 * pow6;
    result += (seq_t)9660 * pow5;
    result += (seq_t)48552 * pow4;
    result += (seq_t)142632 * pow3;
    result += (seq_t)280140 * pow2;
    result += (seq_t)341796;
    return result;
}
