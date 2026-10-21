/*
 * A213392  Number of (w,x,y) with all terms in {0,...,n} and 2*max(w,x,y) >=
 *          3*min(w,x,y).
 * https://oeis.org/A213392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^3 + 27*n^2 + 21*n + 6*n*(((n+1) mod 3) mod 2) + 7 +
 *            2*((2*n+1) mod 3))/9
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* Compute the small moduli without risking overflow */
    idx_t mod_n3 = n % (idx_t)3;                    /* n mod 3                  */
    idx_t mod1   = (mod_n3 + (idx_t)1) % (idx_t)3; /* (n+1) mod 3               */
    idx_t mod1_2 = mod1 % (idx_t)2;                 /* (((n+1) mod 3) mod 2)     */
    idx_t mod2   = ((mod_n3 * (idx_t)2) + (idx_t)1) % (idx_t)3; /* (2*n+1) mod 3  */

    seq_t N2 = N * N;
    seq_t N3 = N2 * N;

    seq_t term = (seq_t)8 * N3
               + (seq_t)27 * N2
               + (seq_t)21 * N
               + (seq_t)7
               + (seq_t)6 * N * (seq_t)mod1_2
               + (seq_t)2 * (seq_t)mod2;

    return term / (seq_t)9;
}
