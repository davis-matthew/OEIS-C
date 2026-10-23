/*
 * A166800  Number of nX2 1..7 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          nondecreasing order.
 * https://oeis.org/A166800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n-1)(n-2)(n-3)*P(n) / 14968800, where P(n) = n^9 + 78
 *            n^8 + 3405 n^7 + 93498 n^6 + 1514529 n^5 + 12368202 n^4 +
 *            25097225 n^3 – 108018018 n^2 – 1797300 n – 2494800
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;

    /* cast to seq_t early to avoid promotion issues */
    seq_t nn = (seq_t)n;

    /* (n-1)(n-2)(n-3) */
    seq_t prod = (nn - 1) * (nn - 2);
    prod = prod * (nn - 3);

    /* powers of n up to n^9 */
    seq_t pow2 = nn * nn;
    seq_t pow3 = pow2 * nn;
    seq_t pow4 = pow3 * nn;
    seq_t pow5 = pow4 * nn;
    seq_t pow6 = pow5 * nn;
    seq_t pow7 = pow6 * nn;
    seq_t pow8 = pow7 * nn;
    seq_t pow9 = pow8 * nn;

    /* polynomial P(n) */
    seq_t poly = pow9;
    poly += (seq_t)78 * pow8;
    poly += (seq_t)3405 * pow7;
    poly += (seq_t)93498 * pow6;
    poly += (seq_t)1514529 * pow5;
    poly += (seq_t)12368202 * pow4;
    poly += (seq_t)25097225 * pow3;
    poly -= (seq_t)108018018 * pow2;
    poly -= (seq_t)1797300 * nn;
    poly -= (seq_t)2494800;

    /* final division */
    const seq_t denom = (seq_t)14968800;
    return prod * poly / denom;
}
