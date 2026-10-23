/*
 * A024598  a(n) = s(1)*s(n) + s(2)*s(n-1) + ... + s(k)*s(n+1-k), where k =
 *          floor((n+1)/2), s = (odd natural numbers).
 * https://oeis.org/A024598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 3*n^2 + 2*n - 3*n^2*(-1)^n)/12
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using integer arithmetic; the value is always an integer for n >= 1 */

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1).  For safety, handle n==0 as 0 (unused). */
    if (n == 0) return (seq_t)0;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t num;

    if ((n & 1ULL) == 0) {          /* n even: (-1)^n = 1 */
        num = 4 * n3 + 2 * nn;
    } else {                        /* n odd: (-1)^n = -1 */
        num = 4 * n3 + 6 * n2 + 2 * nn;
    }

    return num / 12;
}
