/*
 * A212762  Number of (w,x,y,z) with all terms in {0,...,n}, w and x odd, y
 *          even.
 * https://oeis.org/A212762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*( 2*n^3 + 5*n^2 + 3*n + 1 - (n^2 + 3*n + 1)*(-1)^n
 *            ) / 16 (direct evaluation of the closed–form expression from
 *            OEIS A212762)
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;      // cast index to seq_t
    seq_t N1 = N + (seq_t)1;  // n+1

    seq_t n2 = N * N;         // n^2
    seq_t n3 = n2 * N;        // n^3

    seq_t C = (seq_t)2 * n3 + (seq_t)5 * n2 + (seq_t)3 * N + (seq_t)1;
    seq_t D = n2 + (seq_t)3 * N + (seq_t)1;

    seq_t T;
    if ((n & (idx_t)1) == 0) {          // n even : (-1)^n = 1
        T = C - D;                      // C - D
    } else {                            // n odd  : (-1)^n = -1
        T = C + D;                      // C + D
    }

    return (N1 * T) / (seq_t)16;        // final division by 16
}
