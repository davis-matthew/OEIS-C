/*
 * A163547  The square of the distance from the origin to the n-th term in the
 *          type I Hilbert's Hamiltonian walk A163357.
 * https://oeis.org/A163547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000290(A059252(n)) + A000290(A059253(n))
 * Range:     u8 n <= 104, u16 n <= 9067, u32 n <= 9067, u64 n <= 9067,
 *            u128 n <= 9067, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute Hilbert curve coordinates (x, y) for index n. */
    idx_t x = 0, y = 0;

    /* Determine side length 2^m that is > sqrt(n) */
    idx_t temp = n;
    unsigned bits = 0;
    while (temp > 0) {
        temp >>= 1;
        bits++;
    }
    idx_t side = 1ULL << ((bits + 1) / 2);

    idx_t t = n;
    for (idx_t s = 1; s < side; s <<= 1) {
        idx_t rx = (t >> 1) & 1;
        idx_t ry = (t ^ rx) & 1;
        if (ry == 0) {
            if (rx == 1) {
                x = s - 1 - x;
                y = s - 1 - y;
            }
            idx_t tmp = x;
            x = y;
            y = tmp;
        }
        x += s * rx;
        y += s * ry;
        t >>= 2;
    }

    /* Compute squared distance from origin. */
    seq_t xx = (seq_t)x;
    seq_t yy = (seq_t)y;
    return xx * xx + yy * yy;
}
