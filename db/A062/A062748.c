/*
 * A062748  Fourth column (r=3) of FS(3) staircase array A062745.
 * https://oeis.org/A062748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+4,3) - 1
 * Range:     u8 n <= 8, u16 n <= 70, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t f1 = (seq_t)n + 2;
    seq_t f2 = (seq_t)n + 3;
    seq_t f3 = (seq_t)n + 4;

    /* Divide out a factor of 2 */
    if (f1 % 2 == 0) {
        f1 /= 2;
    } else if (f2 % 2 == 0) {
        f2 /= 2;
    } else {
        f3 /= 2;
    }

    /* Divide out a factor of 3 */
    if (f1 % 3 == 0) {
        f1 /= 3;
    } else if (f2 % 3 == 0) {
        f2 /= 3;
    } else {
        f3 /= 3;
    }

    seq_t prod = f1 * f2;
    prod = prod * f3;

    return prod - 1;
}
