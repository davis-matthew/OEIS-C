/*
 * A090392  Seventh diagonal (m=6) of triangle A084938; a(n) = A084938(n+6,n)
 *          = (n^6 + 45*n^5 + 925*n^4 + 11475*n^3 + 92314*n^2 + 413640*n)/720.
 * https://oeis.org/A090392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^6 + 45*n^5 + 925*n^4 + 11475*n^3 + 92314*n^2 +
 *            413640*n)/720
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0, a(1) = 720, a(2) = 1812, a(3) = 3428, a(4) = 5768, a(5) = 9090, a(6) = 13721 */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    seq_t n6 = n5 * nn;

    seq_t num = n6;
    num = num + ((seq_t)45   * n5);
    num = num + ((seq_t)925  * n4);
    num = num + ((seq_t)11475 * n3);
    num = num + ((seq_t)92314 * n2);
    num = num + ((seq_t)413640 * nn);

    return num / 720;
}
