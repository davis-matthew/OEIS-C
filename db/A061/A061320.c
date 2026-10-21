/*
 * A061320  Column 4 of A061314.
 * https://oeis.org/A061320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n*(n+1)*(n^8+4*n^7+14*n^6+28*n^5+77*n^4+112*n^3+196*n^2+144*n+2304)
 *            *(n^2+n+4)*(n^4+2*n^3+5*n^2+4*n+36) / 331776 (All arithmetic is
 *            exact; intermediate multiplication is performed in __int128 to
 *            avoid overflow for all n whose result fits in seq_t.)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 8, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* part1: n*(n+1) */
    seq_t part1 = nn * (nn + (seq_t)1);

    /* part2: n^8 + 4*n^7 + 14*n^6 + 28*n^5 + 77*n^4 + 112*n^3
             + 196*n^2 + 144*n + 2304 */
    seq_t n2 = nn * nn;               /* n^2   */
    seq_t n3 = n2 * nn;               /* n^3   */
    seq_t n4 = n3 * nn;               /* n^4   */
    seq_t n5 = n4 * nn;               /* n^5   */
    seq_t n6 = n5 * nn;               /* n^6   */
    seq_t n7 = n6 * nn;               /* n^7   */
    seq_t n8 = n7 * nn;               /* n^8   */
    seq_t part2 = n8
                  + (seq_t)4 * n7
                  + (seq_t)14 * n6
                  + (seq_t)28 * n5
                  + (seq_t)77 * n4
                  + (seq_t)112 * n3
                  + (seq_t)196 * n2
                  + (seq_t)144 * nn
                  + (seq_t)2304;

    /* part3: n^2 + n + 4 */
    seq_t part3 = n2 + nn + (seq_t)4;

    /* part4: n^4 + 2*n^3 + 5*n^2 + 4*n + 36 */
    seq_t part4 = n4
                  + (seq_t)2 * n3
                  + (seq_t)5 * n2
                  + (seq_t)4 * nn
                  + (seq_t)36;

    /* multiply all parts using 128‑bit arithmetic, then divide by 331776 */
    __int128 prod = (__int128)part1 * (__int128)part2;
    prod = prod * (__int128)part3;
    prod = prod * (__int128)part4;
    prod = prod / 331776;

    return (seq_t)prod;
}
