/*
 * A195045  Concentric 13-gonal numbers.
 * https://oeis.org/A195045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*n^2/4 + 9*((-1)^n - 1)/8
 * Range:     u8 n <= 4, u16 n <= 142, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1ULL) == 0) {
        /* Even n:  a(n) = 13*n^2/4  (n is even, n^2 divisible by 4) */
        seq_t res = (nn * nn) / (seq_t)4;   /* safe, division exact */
        res *= (seq_t)13;
        return res;
    }
    /* Odd n:  a(n) = (13*n^2 - 9)/4 */
    seq_t sq  = nn * nn;
    seq_t num = sq * (seq_t)13 - (seq_t)9;
    return num / (seq_t)4;
}
