/*
 * A231304  Recurrence a(n) = a(n-2) + n^M for M=5, starting with a(0)=0,
 *          a(1)=1.
 * https://oeis.org/A231304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (n-2k)^5
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 60, u64 n <= 2458,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t x = (seq_t)n - (seq_t)(2 * k);
        seq_t term = x * x * x * x * x;
        sum += term;
    }
    return sum;
}
