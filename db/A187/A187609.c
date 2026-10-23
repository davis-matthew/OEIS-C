/*
 * A187609  Number of 5-step one space for components leftwards or up, two
 *          space for components rightwards or down asymmetric quasi-bishop's
 *          tours (antidiagonal moves become knight moves) on an n X n board
 *          summed over all starting positions.
 * https://oeis.org/A187609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 193*n^2 - 1700*n + 3620 for n > 7 Initial values for n =
 *            1..7: 0, 0, 0, 8, 130, 500, 1274
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[7] = {
        (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)8,
        (seq_t)130, (seq_t)500, (seq_t)1274
    };

    if (n <= 7) {
        return init[n - 1];
    }

    seq_t N = (seq_t)n;
    seq_t result = (seq_t)193 * N * N;
    result -= (seq_t)1700 * N;
    result += (seq_t)3620;
    return result;
}
