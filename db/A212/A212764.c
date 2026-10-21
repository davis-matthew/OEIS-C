/*
 * A212764  Number of (w,x,y,z) with all terms in {0,...,n}, w, x and y odd,
 *          and z odd.
 * https://oeis.org/A212764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (2*n^4+10*n^3+18*n^2+12*n+1+(2*n^3+6*n^2+4*n-1)*(-1)^n)/32 for n
 *            >= 0
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == (idx_t)0) return 0;
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t N4 = N3 * N;
    seq_t num = 2 * N4 + 10 * N3 + 18 * N2 + 12 * N + 1;
    seq_t extra = 2 * N3 + 6 * N2 + 4 * N - 1;
    if (n & 1) {          /* n is odd: subtract extra */
        num = num - extra;
    } else {              /* n is even: add extra */
        num = num + extra;
    }
    return num >> 5;       /* divide by 32 */
}
