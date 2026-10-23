/*
 * A029290  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^10)*(1-x^12)).
 * https://oeis.org/A029290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+45*n^2+312*n+1735)/10800 - (n mod 2)*n/240 +
 *            ((2*n^2+1) mod 3)*(n+12)/36 + ((2*n^2+3) mod 5)*n/50 +
 *            ((4*n^2+3*n+4) mod 5)/5)
 * Range:     u8 none, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast all integral constants to seq_t to avoid overflow in C++ */
    seq_t N = (seq_t)n;

    /* termA numerator: (n^3 + 45*n^2 + 312*n + 1735) */
    seq_t termA = N * N * N
                 + (seq_t)45 * N * N
                 + (seq_t)312 * N
                 + (seq_t)1735;

    /* termB numerator: (n mod 2) * n * 45  (240 = 10800/45) */
    seq_t termB = ((seq_t)(n % 2) * N) * (seq_t)45;

    /* termC numerator: ((2*n^2+1) mod 3) * (n+12) * 300  (36 = 10800/300) */
    seq_t termC = ((seq_t)(( (seq_t)2 * N * N + (seq_t)1 ) % (seq_t)3))
                 * (N + (seq_t)12)
                 * (seq_t)300;

    /* termD numerator: ((2*n^2+3) mod 5) * n * 216  (50 = 10800/216) */
    seq_t termD = ((seq_t)(( (seq_t)2 * N * N + (seq_t)3 ) % (seq_t)5))
                 * N
                 * (seq_t)216;

    /* termE numerator: ((4*n^2+3*n+4) mod 5) * 2160  (5 = 10800/2160) */
    seq_t termE = ((seq_t)(( (seq_t)4 * N * N + (seq_t)3 * N + (seq_t)4 ) % (seq_t)5))
                 * (seq_t)2160;

    /* combine numerator and divide by the common denominator 10800 */
    seq_t numerator = termA - termB + termC + termD + termE;
    return numerator / (seq_t)10800;
}
