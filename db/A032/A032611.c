/*
 * A032611  Concatenation of n and n + 6 or {n,n+6}.
 * https://oeis.org/A032611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(n, n+6)
 * Range:     u8 n <= 3, u16 n <= 93, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* number of digits of n+6 */
    idx_t m = n + 6;
    idx_t d = 0;
    idx_t tmp = m;
    do {
        tmp /= 10;
        ++d;
    } while(tmp > 0);

    /* 10^d as seq_t */
    seq_t pow = 1;
    for(idx_t i = 0; i < d; ++i) {
        pow *= 10;
    }

    /* concatenation: n * 10^d + (n+6) */
    return (seq_t)n * pow + (seq_t)m;
}
