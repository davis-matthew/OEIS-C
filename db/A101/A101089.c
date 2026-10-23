/*
 * A101089  Second partial sums of fourth powers (A000583).
 * https://oeis.org/A101089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)^2*(n+2)*(2*n*(n+2)-1)/60
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 70, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t f[5];
    f[0] = nn;
    f[1] = nn + (seq_t)1;
    f[2] = nn + (seq_t)1;
    f[3] = nn + (seq_t)2;
    f[4] = (seq_t)2 * nn * (nn + (seq_t)2) - (seq_t)1;

    seq_t divs[4] = {(seq_t)2, (seq_t)2, (seq_t)3, (seq_t)5};
    for (int i = 0; i < 4; ++i) {
        seq_t d = divs[i];
        for (int j = 0; j < 5; ++j) {
            if (f[j] % d == 0) {
                f[j] /= d;
                break;
            }
        }
    }

    seq_t result = (seq_t)1;
    for (int j = 0; j < 5; ++j)
        result *= f[j];
    return result;
}
