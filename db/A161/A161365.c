/*
 * A161365  a(n) = 3/2 + 5*n - 5*(-1)^n/2.
 * https://oeis.org/A161365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n - 1 for even n, 5*n + 4 for odd n
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    if ((n & 1ull) == 0) {    /* n is even */
        return 5 * sn - 1;
    } else {                  /* n is odd */
        return 5 * sn + 4;
    }
}
