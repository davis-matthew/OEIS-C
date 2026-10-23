/*
 * A131730  a(4n) = n, a(4n+1) = -n-1, a(4n+2) = n+1, a(4n+3) = -n.
 * https://oeis.org/A131730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(4n) = n, a(4n+1) = -n-1, a(4n+2) = n+1, a(4n+3) = -n
 * Range:     i8 n <= 509, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 4;          /* integer division, floor(n/4) */
    idx_t r = n & 3;          /* n % 4, faster with bitwise AND */

    switch (r) {
    case 0:
        return (seq_t)k;          /* a(4k) = k */
    case 1:
        return -((seq_t)k) - 1;   /* a(4k+1) = -(k+1) */
    case 2:
        return ((seq_t)k) + 1;    /* a(4k+2) = k+1 */
    case 3:
        return -((seq_t)k);       /* a(4k+3) = -k */
    default:                      /* unreachable, but keep compiler happy */
        return 0;
    }
}
