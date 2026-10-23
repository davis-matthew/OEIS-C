/*
 * A136184  Number of metacyclic groups of order 2^n.
 * https://oeis.org/A136184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n^3 + 48*n^2 - c*n + d) / 72) + (n<4?(n-4):0) where c =
 *            168 if n is even, else 177; d = 432 if n mod 3 == 0, 416 if n
 *            mod 3 == 1, 424 if n mod 3 == 2
 * Range:     u8 none, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t for all arithmetic */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    seq_t c = (n % 2 == 0) ? (seq_t)168 : (seq_t)177;
    seq_t d;
    switch (n % 3) {
        case 0: d = 432; break;
        case 1: d = 416; break;
        default: d = 424; break;
    }

    seq_t num = n3 + (48 * n2) - (c * nn) + d;
    seq_t val = num / (seq_t)72;

    if (n < 4)
        val += (seq_t)n - 4;  /* adjust for n = 1,2,3 */

    return val;
}
