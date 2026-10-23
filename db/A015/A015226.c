/*
 * A015226  Even hexagonal pyramidal numbers.
 * https://oeis.org/A015226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+(-1)^n+5)*(4*n+(-1)^n+7)*(8*n+2*(-1)^n+9)/24
 * Range:     u8 none, u16 n <= 6, u32 n <= 344, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Even hexagonal pyramidal numbers. */
seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t term1, term2, term3;

    if (n & 1) {                /* n is odd */
        term1 = 4 * nseq + 4;   /* 4*n + (-1)^n + 5 = 4*n - 1 + 5 */
        term2 = 4 * nseq + 6;   /* 4*n + (-1)^n + 7 = 4*n - 1 + 7 */
        term3 = 8 * nseq + 7;   /* 8*n + 2*(-1)^n + 9 = 8*n - 2 + 9 */
    } else {                    /* n is even */
        term1 = 4 * nseq + 6;   /* 4*n + (+1) + 5 */
        term2 = 4 * nseq + 8;   /* 4*n + (+1) + 7 */
        term3 = 8 * nseq + 11;  /* 8*n + 2* (+1) + 9 */
    }

    seq_t num = term1 * term2 * term3;
    return num / 24;
}
