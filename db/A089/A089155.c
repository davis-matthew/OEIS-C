/*
 * A089155  a(n) = (2*n)!*(Integral_{x=0..sqrt(2/3)} 1/(1-x^2)^(n+1/2)
 *          dx)/((n!*2^n)*sqrt(2)).
 * https://oeis.org/A089155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8*n-11)*a(n-1) - 6*(2*n-3)*(n-2)*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence initial values: a(1) = 1, a(2) = 5              */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    seq_t a_nm2 = (seq_t)1;    /* a(1) */
    seq_t a_nm1 = (seq_t)5;    /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t part1 = (seq_t)(8*i - 11) * a_nm1;
        seq_t part2 = (seq_t)6 * (seq_t)(2*i - 3) * (seq_t)(i - 2) * a_nm2;
        seq_t a_i   = part1 - part2;

        a_nm2 = a_nm1;
        a_nm1 = a_i;
    }
    return a_nm1;
}
