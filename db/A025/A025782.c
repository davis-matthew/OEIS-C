/*
 * A025782  Expansion of 1/((1-x)*(1-x^6)*(1-x^7)).
 * https://oeis.org/A025782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 14*n + 7*(2+(n mod 6))*(6-(n mod 6)) - 6*(n mod
 *            7)*(7-(n mod 7)))/84
 * Range:     u8 n <= 11, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* small remainders */
    idx_t m6 = n % 6;
    idx_t m7 = n % 7;

    /* cast once to seq_t for arithmetic */
    seq_t sn      = (seq_t)n;
    seq_t term1   = sn * sn;                      /* n^2 */
    seq_t term2   = (seq_t)14 * sn;                /* 14*n */
    seq_t term3   = (seq_t)7 * (seq_t)(2 + m6) * (seq_t)(6 - m6);   /* 7*(2+m6)*(6-m6) */
    seq_t term4   = (seq_t)6 * (seq_t)m7 * (seq_t)(7 - m7);          /* 6*m7*(7-m7) */

    seq_t num = term1 + term2 + term3 - term4;
    seq_t res = num / (seq_t)84;
    return res;
}
