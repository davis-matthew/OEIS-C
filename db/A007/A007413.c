/*
 * A007413  A squarefree (or Thue-Morse) ternary sequence: closed under
 *          1->123, 2->13, 3->2. Start with 1.
 * https://oeis.org/A007413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 if the 2-adic valuation of n is odd (a(n) mod 2 =
 *            A035263(n)); otherwise n = A003159(k), where k = #{m <= n :
 *            v2(m) even}, and a(n) = 1 if k is odd, a(n) = 3 if k is even
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n;
    int v = 0;
    while ((m & 1) == 0) {
        m >>= 1;
        v++;
    }
    if (v & 1)
        return (seq_t)2;
    /* count m <= n with even 2-adic valuation */
    idx_t k = 0;
    idx_t p = n;
    int j = 0;
    while (p > 0) {
        idx_t q = p >> 1;
        if ((j & 1) == 0)
            k += p - q;
        p = q;
        j++;
    }
    return (k & 1) ? (seq_t)1 : (seq_t)3;
}
