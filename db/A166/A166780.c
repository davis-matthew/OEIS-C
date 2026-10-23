/*
 * A166780  Number of nX2 1..7 arrays containing at least one of each value,
 *          all equal values connected, and rows considered as a single number
 *          in nondecreasing order.
 * https://oeis.org/A166780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   * a(n) = 0 for n < 4 * a(n) = ((n-1)(n-2)(n-3) * * (2*n^9 +
 *            156*n^8 + 6546*n^7 + 168582*n^6 + 2509143*n^5 * + 18010344*n^4 +
 *            22858999*n^3 * - 183485232*n^2 + 74391660*n * - 2494800)) /
 *            14968800 * for n ≥ 4. * The division is exact for all integer
 *            n≥4
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 4 the sequence is defined as 0. */
    if (n < 4) return 0;

    /* Convert to seq_t for arbitrary‑precision arithmetic. */
    seq_t ns = (seq_t)n;

    /* Evaluate the degree‑9 polynomial P(n) by Horner's rule. */
    seq_t P = 2;
    P = P * ns + 156;
    P = P * ns + 6546;
    P = P * ns + 168582;
    P = P * ns + 2509143;
    P = P * ns + 18010344;
    P = P * ns + 22858999;
    P = P * ns + (seq_t)(-183485232);
    P = P * ns + 74391660;
    P = P * ns + (seq_t)(-2494800);

    /* Compute the product (n-1)(n-2)(n-3) */
    seq_t a1 = (seq_t)(n - 1);
    seq_t a2 = (seq_t)(n - 2);
    seq_t a3 = (seq_t)(n - 3);

    /* Numerator: (n-1)(n-2)(n-3) * P(n) */
    seq_t num = a1 * a2;
    num = num * a3;
    num = num * P;

    /* Denominator 14968800 = 32/(12)! */
    static const seq_t DEN = 14968800;

    /* Exact division. */
    return num / DEN;
}
