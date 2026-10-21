/*
 * A079810  Sums of diagonals (upward from left to right) of the triangle
 *          shown in A079809.
 * https://oeis.org/A079810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4k) = 3*k*k a(4k+1) = a(4k+2) = 3*k*k + 4*k + 1 a(4k+3) =
 *            3*k*k + 8*k + 5 where k is computed from n as shown in each
 *            branch
 * Range:     u8 n <= 34, u16 n <= 590, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)              /* index starts at 1, undefined below */
        return 0;

    switch ((int)(n & 3u)) {            /* n % 4, using bitwise AND as 4 is a power of two */
    case 0: { /* n = 4k */
        idx_t k = n >> 2;                /* n / 4 */
        seq_t K = (seq_t)k;
        return 3 * K * K;
    }
    case 1: case 2: { /* n = 4k+1 or 4k+2 */
        idx_t k = (n - 1) >> 2;          /* (n-1)/4 */
        seq_t K = (seq_t)k;
        return 3 * K * K + 4 * K + 1;
    }
    case 3: { /* n = 4k+3 */
        idx_t k = (n - 3) >> 2;          /* (n-3)/4 */
        seq_t K = (seq_t)k;
        return 3 * K * K + 8 * K + 5;
    }
    }
    return 0; /* unreachable, but keeps compiler happy */
}
