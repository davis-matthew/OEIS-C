/*
 * A186856  Number of 7-step knight's tours on an (n+2) X (n+2) board summed
 *          over all starting positions.
 * https://oeis.org/A186856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 126440*n^2 - 855176*n + 1332608 (for n >= 10), otherwise
 *            the values are taken from a small lookup table
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 187, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 10 && n >= 1) {
        /* 1 ≤ n ≤ 9 */
        switch (n) {
            case 1: return (seq_t)16;
            case 2: return (seq_t)1280;
            case 3: return (seq_t)21664;
            case 4: return (seq_t)118480;
            case 5: return (seq_t)372384;
            case 6: return (seq_t)847520;
            case 7: return (seq_t)1584576;
            case 8: return (seq_t)2596480;
            case 9: return (seq_t)3879696;
        }
    }
    /* n >= 10 (or n == 0, just fall through) */
    seq_t nn = (seq_t)n;
    return (seq_t)126440 * nn * nn - (seq_t)855176 * nn + (seq_t)1332608;
}
