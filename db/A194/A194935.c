/*
 * A194935  Number of lower triangles of a 6 X 6 0..n array with no element
 *          differing from any of its horizontal or vertical neighbors by more
 *          than one.
 * https://oeis.org/A194935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 116330103*n - 217580911 for n > 8; a(1) … a(8) are taken
 *            from the sequence data
 * Range:     u8 none, u16 none, u32 n <= 38, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first eight values of the sequence */
    static const seq_t tbl[8] = {
        (seq_t)2097152,
        (seq_t)43337009,
        (seq_t)136403494,
        (seq_t)248381815,
        (seq_t)364130672,
        (seq_t)480403525,
        (seq_t)596729942,
        (seq_t)713059915
    };

    if (n <= 8u)
        return tbl[n - 1];

    seq_t sn = (seq_t)n;
    return (seq_t)116330103 * sn - (seq_t)217580911;
}
