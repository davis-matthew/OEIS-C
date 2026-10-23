/*
 * A101855  a(n) = n*(n+1)*(n+2)*(n+4)*(n+23)/120.
 * https://oeis.org/A101855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+4)*(n+23)/120
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset – a(1) is first term */
    if (n == 0) return 0;

    seq_t f[5];
    f[0] = (seq_t)n;
    f[1] = (seq_t)n + 1;
    f[2] = (seq_t)n + 2;
    f[3] = (seq_t)n + 4;
    f[4] = (seq_t)n + 23;

    /* 120 = 5 * 3 * 2 * 2 * 2 – divide once by each factor */
    const unsigned divs[5] = {5, 3, 2, 2, 2};
    for (int di = 0; di < 5; ++di) {
        unsigned d = divs[di];
        for (int i = 0; i < 5; ++i) {
            if (f[i] % d == 0) {
                f[i] /= d;
                break;
            }
        }
    }

    seq_t result = 1;
    for (int i = 0; i < 5; ++i) {
        result *= f[i];
    }

    return result;
}
