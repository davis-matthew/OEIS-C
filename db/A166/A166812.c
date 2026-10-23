/*
 * A166812  Number of n X 7 1..2 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          nondecreasing order.
 * https://oeis.org/A166812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+7,7) - 2, where C is the binomial coefficient
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 75, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C(n+7,7) multiplicatively. */
    seq_t res = 1;
    for (idx_t i = 1; i <= 7; ++i)
        /* Use multiplication first then exact division by i. */
        res = res * (seq_t)(n + i) / (idx_t)i;

    /* At least one of each value; subtract the two forbidden arrays. */
    if (res >= 2)
        res = res - 2;
    else
        res = 0; /* safety for n = 0, if ever called */

    return res;
}
