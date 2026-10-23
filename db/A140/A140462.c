/*
 * A140462  Turan's upper bound on the number of triangles of a simplicial
 *          complex of dimension two for which every minimal non-face has
 *          three vertices.
 * https://oeis.org/A140462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5*k^3 - 3*k^2)/2 if n = 3*k, (5*k^3 + 2*k^2 - k)/2 if n
 *            = 3*k+1, (5*k^3 + 7*k^2 + 2*k)/2 if n = 3*k+2 where k = n / 3
 * Range:     u8 n <= 11, u16 n <= 88, u32 n <= 2898, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k_idx = n / 3;
    seq_t k = (seq_t)k_idx;
    seq_t k2 = k * k;
    seq_t k3 = k2 * k;
    seq_t num;

    switch (n % 3) {
        case 0: /* n = 3*k */
            num = 5 * k3 - 3 * k2;
            break;
        case 1: /* n = 3*k + 1 */
            num = 5 * k3 + 2 * k2 - k;
            break;
        case 2: /* n = 3*k + 2 */
            num = 5 * k3 + 7 * k2 + 2 * k;
            break;
        default:
            num = 0; /* unreachable */
            break;
    }
    return num >> 1; /* divide by 2; numerator is always even */
}
