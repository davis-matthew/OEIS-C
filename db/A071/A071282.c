/*
 * A071282  Denominators of Peirce sequence of order 3.
 * https://oeis.org/A071282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = a(n-6) for n > 5. Initial values: a(0)=2, a(1)=3, a(2)=1,
 *            a(3)=3, a(4)=2, a(5)=3. Hence a(n) repeats every 6 terms
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[6] = { (seq_t)2, (seq_t)3, (seq_t)1,
                                  (seq_t)3, (seq_t)2, (seq_t)3 };
    return tbl[n % 6];
}
