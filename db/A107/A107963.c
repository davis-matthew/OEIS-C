/*
 * A107963  a(n) = (n+1)*(n+2)*(n+3)*(n+4)*(5*n^2 + 19*n + 15)/360.
 * https://oeis.org/A107963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(n+4)*(5*n^2 + 19*n + 15)/360
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 79, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Build the five factors */
    seq_t f[5];
    f[0] = (seq_t)n + 1;
    f[1] = (seq_t)n + 2;
    f[2] = (seq_t)n + 3;
    f[3] = (seq_t)n + 4;

    /* 5*n^2 + 19*n + 15 */
    seq_t tmp = (seq_t)n;
    tmp = tmp * tmp;                  /* n^2 */
    f[4] = 5 * tmp + 19 * (seq_t)n + 15;

    /* Cancel the denominator 360 = 2^3 * 3^2 * 5
       by dividing the factors in turn.  The product of
       the 5 numbers is always divisible by 360, so the
       division is exact. */
    /* Cancel 2 three times */
    for (int k = 0; k < 3; ++k) {
        for (int i = 0; i < 5; ++i) {
            if (f[i] % 2 == 0) {
                f[i] = f[i] / 2;
                break;
            }
        }
    }
    /* Cancel 3 two times */
    for (int k = 0; k < 2; ++k) {
        for (int i = 0; i < 5; ++i) {
            if (f[i] % 3 == 0) {
                f[i] = f[i] / 3;
                break;
            }
        }
    }
    /* Cancel 5 once */
    for (int i = 0; i < 5; ++i) {
        if (f[i] % 5 == 0) {
            f[i] = f[i] / 5;
            break;
        }
    }

    /* Multiply the reduced factors to obtain the result */
    seq_t res = 1;
    for (int i = 0; i < 5; ++i)
        res = res * f[i];

    return res;
}
