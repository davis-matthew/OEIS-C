/*
 * A025801  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^12)).
 * https://oeis.org/A025801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + n*(17+3*(-1)^n) + 120 + 4*(n+6)*(((n+2) mod
 *            3)-1))/144)
 * Range:     u8 n <= 5, u16 n <= 540, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t A = N * N;
    seq_t B = ((n & 1) == 0) ? N * (seq_t)20 : N * (seq_t)14;
    seq_t C = (seq_t)120;
    seq_t term = ((n % 3) + 2) % 3;          /* ((n+2) mod 3) */
    seq_t D1 = (seq_t)4 * (N + (seq_t)6);    /* 4*(n+6) */
    seq_t num;

    if (term == 2) {          /* ((n+2) mod 3) == 2 -> (+) D1 */
        num = A + B + C + D1;
    } else if (term == 1) {   /* ((n+2) mod 3) == 1 -> 0 * D1 */
        num = A + B + C;
    } else {                  /* ((n+2) mod 3) == 0 -> (-) D1 */
        num = A + B + C;
        num -= D1;
    }

    return num / (seq_t)144;
}
