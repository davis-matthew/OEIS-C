/*
 * A122133  Number of different polyominoes with maximum area of the convex
 *          hull.
 * https://oeis.org/A122133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - 2*n^2 + 4*n)/16 if n mod 4 = 0; a(n) = (n^3 -
 *            2*n^2 + 13*n + 20)/32 if n mod 4 = 1; a(n) = (n^3 - 2*n^2 + 4*n
 *            + 8)/16 if n mod 4 = 2; a(n) = (n^3 - 2*n^2 + 5*n + 8)/32 if n
 *            mod 4 = 3;
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t mod = n & 3ULL;          /* n mod 4 */
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t res;

    switch (mod) {
    case 0:
        res = (N3 - 2 * N2 + 4 * N) / 16;
        break;
    case 1: {
        seq_t num = N3 + 13 * N + 20;
        res = (num - 2 * N2) / 32;
        break;
    }
    case 2:
        res = (N3 - 2 * N2 + 4 * N + 8) / 16;
        break;
    case 3: {
        seq_t num = N3 + 5 * N + 8;
        res = (num - 2 * N2) / 32;
        break;
    }
    }
    return res;
}
