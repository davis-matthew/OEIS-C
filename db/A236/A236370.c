/*
 * A236370  Sum of the largest parts in the partitions of 3n into 3 parts.
 * https://oeis.org/A236370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1+(-1)^n-(1+3*(-1)^n)*n-6*n^2+22*n^3)/16
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 592, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified: if n even: a(n) = (22*n^3 - 6*n^2 - 4*n)/16 */
/*              if n odd:  a(n) = (22*n^3 - 6*n^2 + 2*n - 2)/16 */

seq_t a(idx_t n) {
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t num;

    if ((n & 1) == 0) {         /* n is even */
        num = 22 * N3 - 6 * N2 - 4 * N;
    } else {                    /* n is odd */
        num = 22 * N3 - 6 * N2 + 2 * N - 2;
    }
    return num / 16;
}
