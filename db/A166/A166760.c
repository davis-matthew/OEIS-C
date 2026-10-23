/*
 * A166760  Number of nX2 1..7 arrays containing at least one of each value,
 *          and all equal values connected.
 * https://oeis.org/A166760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/1485)*(n-1)*(n-2)*(n-3)*(64*n^9 - 768*n^8 + 9648*n^7 -
 *            73200*n^6 + 407526*n^5 - 1667892*n^4 + 4855097*n^3 - 9499725*n^2
 *            + 11240730*n - 6237000)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Zero for n < 4 because of the (n-1)(n-2)(n-3) factor. */
    if (n < 4)
        return (seq_t)0;

    /* Treat n as a seq_t to avoid mixed‑type arithmetic. */
    seq_t const nst = (seq_t)n;

    /* Evaluate the 9th‑degree polynomial by Horner’s method. */
    seq_t poly = (seq_t)64;
    poly = poly * nst - (seq_t)768;
    poly = poly * nst + (seq_t)9648;
    poly = poly * nst - (seq_t)73200;
    poly = poly * nst + (seq_t)407526;
    poly = poly * nst - (seq_t)1667892;
    poly = poly * nst + (seq_t)4855097;
    poly = poly * nst - (seq_t)9499725;
    poly = poly * nst + (seq_t)11240730;
    poly = poly * nst - (seq_t)6237000;

    /* Multiply by the (n-1)(n-2)(n-3) factor. */
    seq_t result = (nst - (seq_t)1) * (nst - (seq_t)2) * (nst - (seq_t)3) * poly;

    /* Divide by 1485. The division is exact for all integer n. */
    result = result / (seq_t)1485;

    return result;
}
