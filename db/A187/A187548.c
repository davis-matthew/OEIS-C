/*
 * A187548  Alternating partial sums of L(n)*H(n+1), product of central Lah
 *          number L(n) and Harmonic number H(n+1).
 * https://oeis.org/A187548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * L(k) * H(k+1) where L(k) =
 *            L(2k,k) = (2k)!/k! * binom(2k-1,k-1) (central Lah number). The
 *            values are computed by the recurrence a(0) = 1, a(n) = -a(n-1) +
 *            L(n) * H(n+1) (n >= 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  a(0) = 1  */
    if (n == 0) return 1;

    seq_t L = 1;           /* L(0)          */
    seq_t P = 1;           /* P(0) = L0 * H1 = 1 */
    seq_t a_val = 1;       /* a(0)          */

    for (idx_t i = 1; i <= n; ++i) {
        if (i == 1) {
            /* L(1) = 2,  H2 = 3/2  =>  P(1) = 3,  a(1) = 2 */
            L = 2;
            P = 3;
            a_val = 2;
            continue;
        }

        /* Rnum = 4 * (2i-1)^2  */
        seq_t two_i_minus_1 = (seq_t)(2 * i - 1);
        seq_t Rnum = 4 * two_i_minus_1 * two_i_minus_1;

        /* L(i) = L(i-1) * Rnum / i */
        seq_t L_next = (seq_t)(((__int128)L * Rnum) / i);

        /* P(i) = (Rnum * P(i-1)) / i  +  (Rnum * L(i-1)) / (i*(i+1)) */
        seq_t term1 = (seq_t)(((__int128)Rnum * P) / i);
        seq_t denom = i * (i + 1);
        seq_t term2 = (seq_t)(((__int128)Rnum * L) / denom);
        seq_t P_next = term1 + term2;

        /* a(i) = P(i) - a(i-1)   (=>  -a(i-1) + P(i)) */
        seq_t a_next = P_next - a_val;

        /* update for next iteration */
        L = L_next;
        P = P_next;
        a_val = a_next;
    }

    return a_val;
}
