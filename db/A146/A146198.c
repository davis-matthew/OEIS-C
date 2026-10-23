/*
 * A146198  Number of n X n arrays of squares of integers with every (n-1) X
 *          (n-1) subblock summing to 6 and every element equal to at least
 *          one neighbor.
 * https://oeis.org/A146198
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 80*n^6/9 - 401*n^5/3 + 10112*n^4/9 - 5809*n^3 * +
 *            145814*n^2/9 - 64942*n/3 + 11612 for n >= 7 * a(3) = 0, a(4) =
 *            40, a(5) = 5606, a(6) = 41698
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 21, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3) return 0;
    if (n == 3) return 0;
    if (n == 4) return 40;
    if (n == 5) return 5606;
    if (n == 6) return 41698;

    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    seq_t n3 = n2 * sn;
    seq_t n4 = n3 * sn;
    seq_t n5 = n4 * sn;
    seq_t n6 = n5 * sn;

    seq_t num = (seq_t)80   * n6
              - (seq_t)1203 * n5
              + (seq_t)10112 * n4
              - (seq_t)52281 * n3
              + (seq_t)145814 * n2
              - (seq_t)194826 * sn
              + (seq_t)104508;  /* 9 * 11612 */

    return num / (seq_t)9;
}
