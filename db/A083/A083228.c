/*
 * A083228  A Jacobsthal related sequence.
 * https://oeis.org/A083228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^C(n+2,2) + 1)/3 if C(n+2,2) is odd, * (2^C(n+2,2) -
 *            1)/3 if C(n+2,2) is even. * Here C(n+2,2) = (n+1)(n+2)/2 and all
 *            operations are performed * in the seq_t type
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* t = (n+1)(n+2)/2  (binomial coefficient) */
    idx_t t = ((idx_t)n + 1) * ((idx_t)n + 2) / 2;

    /* compute 2^t by binary exponentiation */
    seq_t pow2 = 1;
    seq_t base = 2;
    idx_t exp = t;
    while (exp > 0) {
        if (exp & 1U) pow2 *= base;
        base *= base;
        exp >>= 1U;
    }

    /* numerator = pow2 +/- 1 depending on parity of t */
    seq_t numer = pow2;
    if (t & 1U) /* t odd -> add 1 */
        numer += 1;
    else        /* t even -> subtract 1 */
        numer -= 1;

    /* divide by 3 to obtain the Jacobsthal number */
    return numer / 3;
}
