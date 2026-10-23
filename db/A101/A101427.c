/*
 * A101427  Number of different cuboids with volume (pq)^n, where p,q are
 *          distinct prime numbers.
 * https://oeis.org/A101427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   if n % 3 == 0 * a(n) = ((n+2)^2 * (n+1)^2 + 12*(floor(n/2)+1)^2
 *            + 8) / 24 * else * a(n) = ((n+2)^2 * (n+1)^2 +
 *            12*(floor(n/2)+1)^2) / 24
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 404, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n1 = (seq_t)n + 1;          /* n+1 */
    seq_t n2 = (seq_t)n + 2;          /* n+2 */
    seq_t N  = n1 * n2;               /* (n+1)*(n+2) */
    seq_t k  = (seq_t)(n / 2 + 1);    /* floor(n/2)+1 */
    seq_t k2 = k * k;
    seq_t tmp = N * N + 12 * k2;
    if (n % 3 == 0)
        tmp += 8;
    return tmp / 24;
}
