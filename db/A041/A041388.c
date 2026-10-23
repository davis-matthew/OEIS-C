/*
 * A041388  Numerators of continued fraction convergents to sqrt(209).
 * https://oeis.org/A041388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 14. Let a_n be the partial quotients of the simple
 *            continued fraction of sqrt(209). Define numerators of
 *            convergents by: h_{-2} = 0, h_{-1} = 1; for n >= 0 : h_n = a_n *
 *            h_{n-1} + h_{n-2}. This function returns h_n (the numerator of
 *            the n‑th convergent)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Constants for sqrt(209) */
    const unsigned long long D  = 209ULL;   /* 209 = D in sqrt(D) */
    const unsigned long long A0 = 14ULL;    /* floor(sqrt(209)) */

    /* Variables for the Pell CF algorithm */
    unsigned long long m = 0ULL;
    unsigned long long d = 1ULL;
    unsigned long long a_cur = A0;          /* current partial quotient */

    /* Numerators */
    seq_t h_prev2 = 0;     /* h_{-2} */
    seq_t h_prev1 = 1;     /* h_{-1} */
    seq_t h_curr  = 0;

    idx_t i = 0;           /* current index */

    while (1) {
        /* Compute numerator h_n */
        h_curr = (seq_t)a_cur * h_prev1 + h_prev2;

        if (i == n)
            return h_curr;     /* desired value */

        /* Shift for next iteration */
        h_prev2 = h_prev1;
        h_prev1 = h_curr;

        /* Advance to next partial quotient a_{n+1} */
        m = d * a_cur - m;
        d = (D - m * m) / d;
        a_cur = (A0 + m) / d;

        ++i;
    }
}
