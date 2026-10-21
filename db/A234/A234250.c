/*
 * A234250  Number of ways to choose 3 points in an n X n X n triangular grid
 *          so that they do not form a 2 X 2 X 2 triangle.
 * https://oeis.org/A234250
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 1)*(n - 2)*(n^4 + 6*n^3 + 13*n^2 + 16*n - 24)/48
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 76, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2U) return 0;
    seq_t z   = (seq_t)n;
    seq_t n1 = z - 1;
    seq_t n2 = z * z;
    seq_t n3 = n2 * z;
    seq_t n4 = n3 * z;
    seq_t P   = n4 + 6 * n3 + 13 * n2 + 16 * z - 24;

    seq_t arr[3] = { n1, z - 2, P };
    int need2 = 4;   /* exponent of 2 in 48 */
    int need3 = 1;   /* exponent of 3 in 48 */

    for (int i = 0; i < 3; ++i)
    {
        while (need2 > 0 && arr[i] % 2 == 0)
        {
            arr[i] /= 2;
            --need2;
        }
        while (need3 > 0 && arr[i] % 3 == 0)
        {
            arr[i] /= 3;
            --need3;
        }
    }

    /* At this point need2 == 0 && need3 == 0 */
    seq_t res = arr[0];
    res *= arr[1];
    res *= arr[2];
    return res;
}
