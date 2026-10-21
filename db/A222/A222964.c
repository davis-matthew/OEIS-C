/*
 * A222964  Numbers k such that 25*k+36 is a square.
 * https://oeis.org/A222964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (50*n*(n-1)+(2*n-1)*(-1)^n+1)/8
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(1) = 0, a(2) = 13, a(3) = 37, a(4) = 76, ...          */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset < 1, but harmless      */
    seq_t nn = (seq_t)n;           /* cast idx_t to seq_t for arithmetic          */
    seq_t term1 = (seq_t)50 * nn * (nn - 1);
    seq_t p = (seq_t)(2 * nn - 1);
    seq_t num = term1 + 1;
    /* (-1)^n is 1 for even n, -1 for odd n */
    if ((n & 1ULL) == 0) {
        num += p;                  /* even n : +p                                */
    } else {
        num -= p;                  /* odd n  : -p (num >= p for n>=1)            */
    }
    return num / 8ULL;             /* exact division by 8                        */
}
