/*
 * A026513  a(n) = t(2+3n), where t = A001285 (Thue-Morse sequence).
 * https://oeis.org/A026513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + t(2+3n) where t(k) is the Thue–Morse function (parity
 *            of ones in the binary expansion of k)
 * Range:     u8 n <= 84, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute k = 3*n + 2 using seq_t to avoid overflow. */
    seq_t k = (seq_t)n * 3 + 2;

    /* Compute parity (popcount mod 2) of k. */
    int p = 0;
    while (k) {
        p ^= (int)(k & 1); /* toggle parity for each set bit */
        k >>= 1;
    }

    /* a(n) is 1 if parity is 0, otherwise 2. */
    return (seq_t)(1 + p);
}
