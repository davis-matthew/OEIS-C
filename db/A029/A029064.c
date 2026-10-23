/*
 * A029064  Expansion of 1/((1-x)*(1-x^4)*(1-x^5)*(1-x^7)).
 * https://oeis.org/A029064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2*n^3 + 51*n^2 + 388*n + 1680) / 1680)
 * Range:     u8 n <= 0, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the generic sequence type to avoid overflow */
    seq_t nn = (seq_t)n;

    /* Compute the cubic polynomial in the seq_t domain */
    seq_t term =
          (seq_t)2      * nn * nn * nn
        + (seq_t)51     * nn * nn
        + (seq_t)388    * nn
        + (seq_t)1680;

    /* The result is always divisible by 1680; integer division is exact */
    return term / (seq_t)1680;
}
