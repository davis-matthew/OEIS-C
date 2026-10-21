/*
 * A212973  Number of triples (w,x,y) with all terms in {0,...,n} and w <=
 *          floor((x+y)/3).
 * https://oeis.org/A212973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 4*n^2 + 5*n + 2 + (((n+1) mod 3) mod 2))/3
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;
    seq_t n3   = n2 * nn;

    /* compute (((n+1) mod 3) mod 2) without overflow */
    seq_t xm3  = (((seq_t)n % (seq_t)3) + (seq_t)1) % (seq_t)3;
    seq_t mod12 = xm3 % (seq_t)2;

    seq_t numer = n3 + (seq_t)4 * n2 + (seq_t)5 * nn + (seq_t)2 + mod12;
    return numer / (seq_t)3;
}
