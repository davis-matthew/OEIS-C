/*
 * A208576  Multiplicative persistence of n in factorial base.
 * https://oeis.org/A208576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8949, u16 n <= 8949, u32 n <= 8949, u64 n <= 8949,
 *            u128 n <= 8949, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A208576 Multiplicative persistence of n in factorial base
   Formula: a(0) = a(1) = 0; for n > 1, a(n) = 1 + a(product of the digits of n in factorial base) */
seq_t a(idx_t n) {
    /* Base cases: 0 and 1 require no further operations */
    if (n < 2) return 0;

    /* Compute the product of the factorial‑base digits of n */
    seq_t product = 1;
    idx_t  temp      = n;
    idx_t  divisor   = 2;          /* (i+1) where i starts at 1 */

    while (temp > 0) {
        idx_t d = temp % divisor;         /* next digit in factorial base  */
        product *= (seq_t)d;               /* multiply into the running product*/
        temp /= divisor;                   /* remove the processed digit   */
        ++divisor;                         /* next divisor is (i+2)         */
    }

    /* Recursively compute persistence for the product */
    return 1 + a((idx_t)product);
}
