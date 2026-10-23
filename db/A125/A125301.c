/*
 * A125301  a(3n) = n, a(3n+1) = (n+2)*a(3n), a(3n+2) = (n+2)*a(3n+1).
 * https://oeis.org/A125301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k) = k a(3k+1) = (k+2) · k a(3k+2) = (k+2)^2 · k where k =
 *            floor(n/3) and n%3 gives the case
 * Range:     u8 n <= 19, u16 n <= 118, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t q = n / 3;          /* k = floor(n/3) */
    idx_t r = n % 3;          /* remainder 0,1,2 */
    seq_t k = (seq_t)q;       /* convert to seq_t for arithmetic */

    switch (r) {
    case 0:                     /* n == 3k */
        return k;

    case 1:                     /* n == 3k + 1 */
        return k * (k + (seq_t)2);

    case 2:                     /* n == 3k + 2 */
        {
            seq_t t = k + (seq_t)2;
            return k * t * t;
        }

    default:                    /* should never happen */
        return 0;
    }
}
