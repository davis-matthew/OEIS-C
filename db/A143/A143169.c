/*
 * A143169  Fourth column of triangle A000369: |S2(-3;n+4,4)|.
 * https://oeis.org/A143169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |S2(-3 ; n+4 , 4)| = (45*P1 - 120*2^n*P2 + 77*P3) / 2
 *            where P1 = ∏_{i=1}^n (4i + 9) P2 = ∏_{i=1}^n (2i + 5) P3 =
 *            ∏_{i=1}^n (4i + 11)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t P1 = 1;
    seq_t P2 = 1;
    seq_t P3 = 1;
    seq_t pow2 = 1;

    for (idx_t i = 1; i <= n; ++i) {
        P1 *= (seq_t)(4*i + 9);
        P2 *= (seq_t)(2*i + 5);
        P3 *= (seq_t)(4*i + 11);
        pow2 <<= 1;          /* pow2 = 2^i   (multiply by 2 each step) */
    }

    seq_t num = (seq_t)45 * P1
              - (seq_t)120 * pow2 * P2
              + (seq_t)77 * P3;

    return num / 2;
}
