/*
 * A182581  (3-adic valuation of n), read mod 2.
 * https://oeis.org/A182581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007949(n) mod 2 (3‑adic valuation of n modulo 2)
 * Range:     u8 n <= 9887, u16 n <= 9887, u32 n <= 9887, u64 n <= 9887,
 *            u128 n <= 9887, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for 0, return 0 for safety */

    /* Compute parity of the exponent of 3 dividing n */
    unsigned int parity = 0;
    idx_t m = n;
    while (m % 3 == 0) {
        parity ^= 1;   /* toggle parity for each factor of 3 */
        m /= 3;
    }
    return (seq_t)parity;
}
