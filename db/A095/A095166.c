/*
 * A095166  Group the natural numbers >= 1 so that the n-th group contains
 *          n(n+1)/2 numbers and obtain the group sum.
 * https://oeis.org/A095166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)*(1 + n*(n+1)*(2n+1)/6)/4
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    if (n_seq == 0) return 0;              /* defined for n ≥ 1; safe fallback */

    /* Compute part = n*(n+1)*(2n+1)/6 with cancellation of 2 and 3 */
    seq_t a1 = n_seq;
    seq_t b1 = n_seq + 1;
    seq_t c1 = n_seq * 2 + 1;              /* 2n+1 */

    /* cancel factor 2 */
    if (a1 % 2 == 0) {
        a1 /= 2;
    } else if (b1 % 2 == 0) {
        b1 /= 2;
    } else {
        c1 /= 2;   /* will not occur because 2n+1 is odd */
    }

    /* cancel factor 3 */
    if (a1 % 3 == 0) {
        a1 /= 3;
    } else if (b1 % 3 == 0) {
        b1 /= 3;
    } else {
        c1 /= 3;
    }

    seq_t part_div6 = a1 * b1 * c1;         /* n*(n+1)*(2n+1)/6 */
    seq_t inside = part_div6 + 1;           /* 1 + ... */

    /* Compute a(n) = n*(n+1)*inside/4 with cancellation of 2×2 */
    seq_t x1 = n_seq;
    seq_t y1 = n_seq + 1;

    /* first division by 2 */
    if (x1 % 2 == 0) {
        x1 /= 2;
    } else if (y1 % 2 == 0) {
        y1 /= 2;
    } else {
        inside /= 2;
    }

    /* second division by 2 */
    if (x1 % 2 == 0) {
        x1 /= 2;
    } else if (y1 % 2 == 0) {
        y1 /= 2;
    } else {
        inside /= 2;
    }

    return x1 * y1 * inside;
}
