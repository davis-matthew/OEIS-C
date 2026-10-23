/*
 * A115000  a(n) = J_2(n) / 24.
 * https://oeis.org/A115000
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = J_2(n)/24
 * Range:     u8 n <= 15, u16 n <= 414, u32 n <= 785, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* J_2(n) = n^2 * Π_{p|n} (1 - 1/p^2)  (Jordan's totient function m = 2) */

seq_t a(idx_t n)
{
    /* Compute J_2(n) as described and divide by 24 to get a(n). */
    /* Work entirely with seq_t to preserve precision. */
    seq_t j2 = (seq_t)n * (seq_t)n;   /* n^2 */

    idx_t temp = n;
    /* handle factor 2 separately to allow odd stepping afterwards */
    if (temp % 2ULL == 0) {
        seq_t p2 = (seq_t)2 * (seq_t)2;  /* 4 */
        j2 = j2 * (p2 - 1) / p2;         /* multiply by (p^2-1) and divide by p^2 */
        while (temp % 2ULL == 0) temp /= 2ULL;
    }
    /* iterate over odd p */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            seq_t p2 = (seq_t)p * (seq_t)p;
            j2 = j2 * (p2 - 1) / p2;
            while (temp % p == 0) temp /= p;
        }
    }
    /* remaining prime factor > 1 */
    if (temp > 1) {
        seq_t p2 = (seq_t)temp * (seq_t)temp;
        j2 = j2 * (p2 - 1) / p2;
    }

    /* final division by 24 */
    return j2 / (seq_t)24;
}
