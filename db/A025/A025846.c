/*
 * A025846  Expansion of 1/((1-x^3)*(1-x^7)*(1-x^9)).
 * https://oeis.org/A025846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 33*n + 273)/378 * - (n+4)*(n mod 3)/27 * +
 *            (4/27)*((n+2) mod 3) * + (2/63)*(((n+7) mod 9)-((n+6) mod 9)))
 * Range:     u8 none, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Common numerator for the exact division by 378 */
    seq_t N = (seq_t)n * (seq_t)n          /* n^2        */
           + (seq_t)33 * (seq_t)n          /* 33*n       */
           + (seq_t)273;                  /* 273        */

    /* Subtract (n+4)*(n mod 3)/27  → *14 because 378/27 = 14 */
    idx_t nm3 = n % 3;
    N -= (seq_t)14 * (seq_t)(n + 4) * (seq_t)nm3;

    /* Add (4/27)*((n+2) mod 3)  → *56 because 4*126/27 = 56 */
    idx_t n_plus2_mod3 = (n + 2) % 3;
    N += (seq_t)56 * (seq_t)n_plus2_mod3;

    /* Add (2/63)*(((n+7) mod 9)-((n+6) mod 9))
       378/63 = 6, so 2*6 = 12 */
    int diff = (int)((n + 7) % 9) - (int)((n + 6) % 9);
    N += (seq_t)12 * (seq_t)diff;

    /* Final result: floor(N / 378) – integer division truncates towards
       zero for positive N, which is the required floor. */
    return N / 378;
}
