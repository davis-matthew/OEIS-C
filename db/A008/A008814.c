/*
 * A008814  Expansion of (1+x^7)/((1-x)^2*(1-x^7)).
 * https://oeis.org/A008814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-5)*floor((n+1)/7) - 7*floor((n+1)/7)^2 + n + 1
 * Range:     u8 n <= 41, u16 n <= 676, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor((n+1)/7) as idx_t */
    idx_t q = (n + 1) / 7;
    seq_t sq_n = (seq_t)n;
    seq_t term1 = 0, term2 = 0;

    if (q != 0) {
        /* For q>0 we are guaranteed 2*n >= 10, so subtraction safe */
        seq_t two_n   = sq_n * 2;
        seq_t temp    = two_n - (seq_t)5;         /* (2*n-5) */
        term1 = temp * (seq_t)q;                  /* (2*n-5)*q   */
        term2 = (seq_t)7 * (seq_t)q * (seq_t)q;   /* 7*q^2       */
    }

    seq_t result = term1 - term2 + sq_n + (seq_t)1;
    return result;
}
