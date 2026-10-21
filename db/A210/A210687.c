/*
 * A210687  The number of triangles in an equipotential triangle divided by
 *          medians into n rows of smaller triangles.
 * https://oeis.org/A210687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (1678*n^3+3117*n^2+88*n-345*Mod[n,2]-320*Mod[n,3]-90*Mod[n,4]-288*Mod[n^3-n^2+n,5])/240
 * Range:     u8 none, u16 n <= 2, u32 n <= 848, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;

    seq_t term1 = (seq_t)1678 * n3;
    seq_t term2 = (seq_t)3117 * n2;
    seq_t term3 = (seq_t)88 * n_seq;

    seq_t mod2 = n % 2;
    seq_t mod3 = n % 3;
    seq_t mod4 = n % 4;

    /* compute (n^3 - n^2 + n) mod 5 without negative wrap */
    seq_t mod5 = ((n3 % 5) + (n_seq % 5) + 5 - (n2 % 5)) % 5;

    seq_t numerator = term1 + term2 + term3
                      - (seq_t)345 * mod2
                      - (seq_t)320 * mod3
                      - (seq_t)90  * mod4
                      - (seq_t)288 * mod5;

    return numerator / (seq_t)240;
}
