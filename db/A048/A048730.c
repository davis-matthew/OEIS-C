/*
 * A048730  Differences between A008589 (multiples of 7) and A048727, a(n) =
 *          ((n*7)-Xmult(n,7)).
 * https://oeis.org/A048730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) is the integer whose i-th binary bit equals 1 when the sum
 *            of the three lower bits of n at positions i-1, i-2, i-3 is at
 *            least 2 (i.e. majority of lower bits). Equivalently a(n) = (7*n)
 *            - Xmult(n,7) where Xmult(n,7) is the result of adding n, 2*n,
 *            and 4*n treated as binary numbers with the u...
 * Range:     u8 n <= 95, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t bit = 1;
    unsigned char w0 = 0, w1 = 0, w2 = 0;  /* window of n's lower bits */
    idx_t m = n;                           /* remaining part of n */

    while (m || w0 || w1 || w2) {
        unsigned sum = w0 + w1 + w2;
        if (sum >= 2) {
            result |= bit;
        }
        bit <<= 1; /* next power of two */

        unsigned char next = (m & 1) ? 1 : 0;
        m >>= 1;
        /* slide window: w2 <- w1, w1 <- w0, w0 <- next */
        w2 = w1;
        w1 = w0;
        w0 = next;
    }
    return result;
}
