/*
 * A124925  Interlaced triples: a(3n+1)=1, a(3n+2) = 2n+3, a(3n+3)=
 *          A028387(n).
 * https://oeis.org/A124925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(3n+1)=1, a(3n+2)=2n+3, a(3n+3)=n^2+3n+1
 * Range:     u8 n <= 47, u16 n <= 767, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n % 3) {
    case 1:
        return (seq_t)1;

    case 2: {
        idx_t k = (n - 2) / 3;
        seq_t ks = (seq_t)k;
        return (seq_t)(2 * ks + 3);
    }

    case 0: {
        idx_t k = n / 3;           /* n = 3k, so k = (n-3)/3 + 1 */
        idx_t kminus1 = k - 1;     /* kminus1 = (n-3)/3 */
        seq_t ks = (seq_t)kminus1;
        return ks * ks + (seq_t)3 * ks + (seq_t)1;
    }
    }
    return (seq_t)0;  /* unreachable for n>=1 */
}
