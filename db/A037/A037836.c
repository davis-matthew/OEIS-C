/*
 * A037836  a(n) = Sum{|d(i)-d(i-1)|: i=1,...,m}, where Sum{d(i)*4^i:
 *          i=0,1,...,m} is the base 4 representation of n.
 * https://oeis.org/A037836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum{|d(i)-d(i-1)| : i = 1,…,m}, where d(i) are the base‑4
 *            digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t absdiff_seq(seq_t a, seq_t b)
{
    return a > b ? a - b : b - a;
}

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 0.  If n < 4, the representation has only one digit, so the sum is 0. */
    idx_t m = n;
    if (m < 4) {
        return 0;
    }

    seq_t prev = m % 4;   /* least significant digit */
    m /= 4;
    seq_t sum = 0;

    while (m > 0) {
        seq_t cur = m % 4;
        sum += absdiff_seq(prev, cur);
        prev = cur;
        m /= 4;
    }
    return sum;
}
